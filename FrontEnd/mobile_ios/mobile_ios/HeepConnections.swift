//
//  HeepConnections.swift
//  mobile_ios
//
//  Created by Jercerb on 4/21/17.
//  Copyright © 2017 Heep. All rights reserved.
//

import SwiftSocket

class HeepConnections {
    public static func testSocket() -> Bool {
        
        let client = TCPClient(address:"192.168.1.181", port:5000)
        
        switch client.connect(timeout:1){
        case .success:
            print("success")
            let bufferarray = HAPIMemoryParser.BuildIsHeepDeviceCOP()
            
            switch client.send(data: bufferarray) {
                
            case .success:
                guard let data = client.read(1024*10) else { return false}
                return HAPIMemoryParser.ParseROP(dump: data)
            case .failure(let error):
                print(error)
            }
        case .failure(let error):
            print(error)
        }
        
        return false
    }
}
