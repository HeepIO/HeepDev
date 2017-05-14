//
//  User.swift
//  mobile_ios
//
//  Created by Jacob Keith on 5/10/17.
//  Copyright © 2017 Heep. All rights reserved.
//

import Foundation
import RealmSwift

class User: Object {
    
    var userID: Int = 0
    var name: String = "Guest"
    var email: String = ""
    var password: String = ""
    var keys = List<Key>()
    var devices = List<Device>()
    var places = List<Place>()
    
    override static func primaryKey() -> String? {
        return "userID"
    }
    
}

class Key: Object {
    var deviceID: Int = 0
    var deviceSecret: Int = 0
    
    override static func primaryKey() -> String? {
        return "deviceID"
    }
    
}

class Place: Object {
    
    dynamic var ssid: String = "placeholder"
    dynamic var bssid: String = "LANMAC"
    dynamic var name: String = "unnamed"
    dynamic var gateway: String = "0.0.0"
    dynamic var x: Int = 100
    dynamic var y: Int = 100
    dynamic var radius: Int = 100
    let devices = List<Device>()
    
    override static func primaryKey() -> String? {
        return "bssid"
    }
}

class Device: Object {
    
    dynamic var name: String = "placeholder"
    dynamic var deviceID: Int = 0
    dynamic var version: Int = 0
    dynamic var iconName: String = "lightbulb"
    dynamic var ipAddress: String = "0.0.0.0"
    let controlList =   List<DeviceControl>() //LinkingObjects(fromType: DeviceControl.self, property: "deviceID")
    dynamic var associatedPlace: String = "wanderer"

    
    override static func primaryKey() -> String? {
        return "deviceID"
    }
    
    
}

class DeviceControl: Object {
    
    dynamic var deviceID: Int = 0//LinkingObjects(fromType: Device.self, property: "deviceID")
    dynamic var controlID: Int = 0
    dynamic var uniqueID: String = ""
    dynamic var controlType: Int = 0
    dynamic var controlDirection: Int = 0
    dynamic var valueLow: Int = 0
    dynamic var valueHigh: Int = 100
    dynamic var valueCurrent: Int = 0
    dynamic var controlName: String = "Default"
    
    override static func primaryKey() -> String? {
        return "uniqueID"
    }
    
}

