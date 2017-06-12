//
//  DeviceTableViewController.swift
//  mobile_ios
//
//  Created by Jercerb on 4/21/17.
//  Copyright © 2017 Heep. All rights reserved.
//

import UIKit
import RealmSwift

class DeviceTableViewController: UITableViewController {
    //MARK: Properties
    
    var notificationToken: NotificationToken!
    let devices: Results<Device>
    var controlTags = [IndexPath]()
    var thisBSSID = ""
    let realm = try! Realm(configuration: configUser)
    
    init(place: Place) {
        devices = realm.objects(Device.self).filter("associatedPlace = %s", place.bssid)
        thisBSSID = place.bssid
        super.init(style: UITableViewStyle.plain)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        self.title = "All Local Devices"
        
        self.initRealmNotifications()
        
        self.navigationController?.isToolbarHidden = false
        
        
        let flush = UIBarButtonItem(barButtonSystemItem: .trash,
                                    target: self,
                                    action: #selector(flushDevices))
        
        let search = UIBarButtonItem(title: "Search For Devices",
                                     style: .plain,
                                     target: self,
                                     action: #selector(searchForHeepDevices))
        
        let spacer = UIBarButtonItem(barButtonSystemItem: .flexibleSpace,
                                     target: nil,
                                     action: nil)
        
        
        self.toolbarItems = [flush, spacer, search, spacer]
        
        
    }
    
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        notificationToken?.stop()
    }

    
    
    override func numberOfSections(in tableView: UITableView) -> Int {
        
        return devices.count

    }
    
    override func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        let view = UIView()
        view.backgroundColor = UIColor.white
        
        let image = UIImage(named: (devices[section].iconName))
        let imageView = UIImageView(image: image)
        imageView.frame = CGRect(x: 5, y: 5, width: 35, height: 35)
        imageView.contentMode = .scaleAspectFit
        view.addSubview(imageView)
        
        let label = UILabel()
        label.text = devices[section].name
        label.frame = CGRect(x: 45, y: 5, width: tableView.frame.size.width, height: 35)
        view.addSubview(label)
        
        let summary = UIButton(type: .detailDisclosure)
        summary.frame = CGRect(x: tableView.frame.size.width - 60, y: 5, width: 60, height: 40)
        summary.tag = section
        summary.addTarget(self, action: #selector(displayDeviceSummary), for: UIControlEvents.primaryActionTriggered)
        
        view.addSubview(summary)
        
        return view
        
    }
    
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        let numControls = devices[section].controlList.count
        return numControls
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        
        let cell = UITableViewCell()
        
        if (devices[indexPath.section].controlList[indexPath.row].controlDirection == 0){
            cell.backgroundColor = UIColor(white: 0.85, alpha: 1.0)
        } else {
            cell.backgroundColor = UIColor(white: 0.95, alpha: 1.0)
        }
        
        let label = UILabel()
        label.text = devices[indexPath.section].controlList[indexPath.row].controlName
        label.frame = CGRect(x: 60, y: 5, width: tableView.frame.size.width, height: 35)
        cell.addSubview(label)
        
        controlTags.append(indexPath)
        
        
        if (devices[indexPath.section].controlList[indexPath.row].controlType == 0){
            let buttonSwitch = UISwitch()
            let thisSwitchState = devices[indexPath.section].controlList[indexPath.row].valueCurrent == 1
            buttonSwitch.tag = controlTags.count - 1
            buttonSwitch.frame = CGRect(x: tableView.frame.size.width - 60, y: 5, width: 100, height: 35)
            buttonSwitch.setOn(thisSwitchState, animated: false)
            buttonSwitch.addTarget(self, action: #selector(DeviceTableViewController.toggle), for: UIControlEvents.valueChanged)
            cell.addSubview(buttonSwitch)
        } else {
            let slider = UISlider()
            slider.tag = controlTags.count - 1
            slider.minimumValue = Float(devices[indexPath.section].controlList[indexPath.row].valueLow)
            slider.maximumValue = Float(devices[indexPath.section].controlList[indexPath.row].valueHigh)
            slider.setValue(Float(devices[indexPath.section].controlList[indexPath.row].valueCurrent), animated: false)
            slider.frame = CGRect(x: tableView.frame.size.width - 160, y: 5, width: 150, height: 35)
            slider.addTarget(self, action: #selector(DeviceTableViewController.sliderUpdate), for: UIControlEvents.valueChanged)
            
            cell.addSubview(slider)
            
        }
        
        return cell
    }
    
    func toggle(sender: UISwitch) {
        
        let realm = try! Realm(configuration: configUser)
        let thisIndexPath = controlTags[sender.tag]
        let thisControlUniqueID = devices[thisIndexPath.section].controlList[thisIndexPath.row].uniqueID
        
        var newValue = 0
        
        if (sender.isOn) {
            newValue = 1
        }
        
        try! realm.write {
            realm.create(DeviceControl.self,
                        value: ["uniqueID": thisControlUniqueID,
                                "valueCurrent": newValue],
                        update: true)
        }
        
        DispatchQueue.global().async {
            HeepConnections().sendValueToHeepDevice(uniqueID: thisControlUniqueID)
        }
        
    }
    
    func sliderUpdate(sender: UISlider) {
        
        let realm = try! Realm(configuration: configUser)
        let thisIndexPath = controlTags[sender.tag]
        let thisControlUniqueID = devices[thisIndexPath.section].controlList[thisIndexPath.row].uniqueID
        
        let newValue = Int(round(sender.value))
        
        try! realm.write {
            realm.create(DeviceControl.self,
                         value: ["uniqueID": thisControlUniqueID,
                                 "valueCurrent": newValue],
                         update: true)
        }
        
        DispatchQueue.global().async {
            HeepConnections().sendValueToHeepDevice(uniqueID: thisControlUniqueID)
        }
        
    }
    
    
    override func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        
        return 45
    }
    
    func displayDeviceSummary(sender: UIButton) {
        print("Display device summary for: \(String(describing: devices[sender.tag].deviceID))")
        let summaryView = DeviceSummaryViewController(device: devices[sender.tag])
        navigationController?.pushViewController(summaryView, animated: true)
    }
    
    func searchForHeepDevices() {
        print("Searching...")
        HeepConnections().SearchForHeepDeviecs()
        
    }
    
    func flushDevices() {
        let realm = try! Realm(configuration: configUser)
        let flushdevices = realm.objects(Device.self).filter("associatedPlace = %s", thisBSSID)
        
        
        try! realm.write {
            for device in flushdevices {
                for control in device.controlList {
                    
                    realm.delete(control)
                }
                
                realm.delete(device)
            }
        }
    }
    
    func initRealmNotifications() {
        let watchPlace = realm.object(ofType: Place.self, forPrimaryKey: thisBSSID)!
        
        notificationToken = watchPlace.addNotificationBlock { changes in
            
            switch changes {
            case .change:
                
                self.tableView.reloadData()
                
                break
            case .error(let error):
                fatalError("\(error)")
                break
            default: break
            }
        }
    }

}