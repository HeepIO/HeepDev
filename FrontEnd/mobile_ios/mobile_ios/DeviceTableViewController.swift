//
//  DeviceTableViewController.swift
//  mobile_ios
//
//  Created by Jercerb on 4/21/17.
//  Copyright © 2017 Heep. All rights reserved.
//

import UIKit

var devices = [Device]()
var lastCount = 0


class DeviceTableViewController: UITableViewController {
    //MARK: Properties

    override func viewDidLoad() {
        super.viewDidLoad()

        loadSampleDevices()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        return devices.count
    }
    
    /*override func tableView(_ tableView: UITableView, titleForHeaderInSection section: Int) -> String {
        
        return devices[section].name
    }*/

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        let numControls = devices[section].controlList.count
        
        return numControls
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        /*let cellIdentifier = "DeviceTableViewCell"
        guard let cell = tableView.dequeueReusableCell(withIdentifier: cellIdentifier, for: indexPath) as? DeviceTableViewCell else {
            fatalError("The dequeued cell is not an instance of DeviceTableViewCell.")
        }*/
        
        let cell = UITableViewCell()
        let label = UILabel()
        label.text = devices[indexPath.section].controlList[indexPath.row].controlName
        label.frame = CGRect(x: 45, y: 5, width: tableView.frame.size.width, height: 35)
        cell.addSubview(label)
        
        
        //fetches the appropriate device or the data source layout. 
        //let device = devices[indexPath.row]
        //cell.nameDevice.text = devices[indexPath.section].controlList[indexPath.row].controlName
        //cell.deviceIconView.image = UIImage(named: "switch")

        return cell
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
        
        return view
        
    }
    
    override func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        
        return 45
    }
    
    
    
    private func loadSampleDevices() {
        /*
        let photo1 = UIImage(named: "lightbulb")
        let photo2 = UIImage(named: "switch")
        let photo3 = UIImage(named: "outlet")
        
        guard let device1 = Device(name: "Blinky", photo: photo1) else { fatalError("Unable to instantiate device1")}
        guard let device2 = Device(name: "ButtonSwitch", photo: photo2) else { fatalError("Unable to instantiate device2")}
        guard let device3 = Device(name: "Outlet", photo: photo3) else { fatalError("Unable to instantiate device3")}
        
        devices += [device1, device2, device3]
        */
    }
    
    
    @IBAction func searchForHeepDevices() {
        // Insert TCP Search Here
        print("Searching...")
        _ = HeepConnections.testSocket()
        //addDevice(device: newDevice)
        
        let dispatchTime = DispatchTime.now() + .seconds(2)
        DispatchQueue.main.asyncAfter(deadline: dispatchTime) {
            
            print("In searchForHeepDevices Action")
            print(devices.count)
            
            self.CheckForNewDevicesAndDisplay()
        }
        

    }
    
    @IBAction func sendValueToHeepDevice() {
        print("Send Value")
    }
    
    private func CheckForNewDevicesAndDisplay() {
        if (devices.count != lastCount) {
            tableView.beginUpdates()
            tableView.insertSections(IndexSet(0...(devices.count-1)), with: .automatic)
            //tableView.insertRows(at: [IndexPath(row: 0, section: devices.count-1)], with: .automatic)
            tableView.endUpdates()
            
            lastCount = devices.count
        }
    }

}
