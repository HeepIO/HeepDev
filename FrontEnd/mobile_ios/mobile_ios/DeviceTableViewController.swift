//
//  DeviceTableViewController.swift
//  mobile_ios
//
//  Created by Jercerb on 4/21/17.
//  Copyright © 2017 Heep. All rights reserved.
//

import UIKit

var devices = [Device]()


class DeviceTableViewController: UITableViewController {
    //MARK: Properties
    
    var controlTags = [IndexPath]()
    var lastCount = 0

    override func viewDidLoad() {
        super.viewDidLoad()
        self.title = "Heep Device List"
        let toolbarContent = UIBarButtonItem()
        toolbarContent.title = "Search for Heep Devices"
        toolbarContent.target = self
        toolbarContent.action = #selector(searchForHeepDevices)
        let spacer = UIBarButtonItem(barButtonSystemItem: .flexibleSpace, target: nil, action: nil)
        self.toolbarItems = [spacer, toolbarContent, spacer]
        searchForHeepDevices()
        
    }
    
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        return devices.count
    }
    
    override func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        let view = UIView()
        view.backgroundColor = UIColor.white
        
        let image = UIImage(named: devices[section].iconName)
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
            buttonSwitch.setOn(thisSwitchState, animated: true)
            buttonSwitch.addTarget(self, action: #selector(DeviceTableViewController.toggle), for: UIControlEvents.valueChanged)
            cell.addSubview(buttonSwitch)
        } else {
            let slider = UISlider()
            slider.tag = controlTags.count - 1
            slider.minimumValue = Float(devices[indexPath.section].controlList[indexPath.row].valueLow)
            slider.maximumValue = Float(devices[indexPath.section].controlList[indexPath.row].valueHigh)
            slider.setValue(Float(devices[indexPath.section].controlList[indexPath.row].valueCurrent), animated: true)
            slider.frame = CGRect(x: tableView.frame.size.width - 160, y: 5, width: 150, height: 35)
            slider.addTarget(self, action: #selector(DeviceTableViewController.sliderUpdate), for: UIControlEvents.valueChanged)
            
            cell.addSubview(slider)
            
        }
        
        return cell
    }
    
    func toggle(sender: UISwitch) {
        
        let thisIndexPath = controlTags[sender.tag]
        
        if (sender.isOn) {
            devices[thisIndexPath.section].controlList[thisIndexPath.row].valueCurrent = 1
            
        } else {
            devices[thisIndexPath.section].controlList[thisIndexPath.row].valueCurrent = 0
        }
        
        DispatchQueue.global().async {
            HeepConnections().sendValueToHeepDevice(thisIndexPath: thisIndexPath)
        }
        
    }
    
    func sliderUpdate(sender: UISlider) {
        
        let thisIndexPath = controlTags[sender.tag]
        
        devices[thisIndexPath.section].controlList[thisIndexPath.row].valueCurrent = Int(round(sender.value))
        DispatchQueue.global().async {
            HeepConnections().sendValueToHeepDevice(thisIndexPath: thisIndexPath)
        }
        
    }
    
    
    override func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        
        return 45
    }
    
    func displayDeviceSummary(sender: UIButton) {
        print("Display device summary for: \(devices[sender.tag].deviceID)")
        let summaryView = DeviceSummaryViewController(device: devices[sender.tag])
        navigationController?.pushViewController(summaryView, animated: true)
    }
    
    
    @IBAction func searchForHeepDevices() {
        print("Searching...")
        HeepConnections().SearchForHeepDeviecs()
        
        let dispatchTime = DispatchTime.now() + .seconds(1)
        DispatchQueue.main.asyncAfter(deadline: dispatchTime) {
            
            print("In searchForHeepDevices Action")
            print(devices.count)
            
            self.CheckForNewDevicesAndDisplay()
        }
        

    }
    
    func CheckForNewDevicesAndDisplay() {
        if (devices.count != 0 && (devices.count - 1) >= lastCount) {
            let previousCount = lastCount
            lastCount = devices.count
            tableView.beginUpdates()
            tableView.insertSections(IndexSet(previousCount...(devices.count-1)), with: .automatic)
            tableView.endUpdates()
        }
    }

}
