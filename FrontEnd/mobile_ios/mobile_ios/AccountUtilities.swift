//
//  AccountUtilities.swift
//  mobile_ios
//
//  Created by Jacob on 6/28/17.
//  Copyright © 2017 Heep. All rights reserved.
//

import Foundation
import RealmSwift

func initializeApp() {
    print("Initializing")
    SyncManager.shared.logLevel = .off
    
    if SyncUser.all.count == 0 {
        
        print("Logging out")
        logoutOfAllRealmUsers()
        configUser = configGuest
        
    } else if SyncUser.all.first?.key == publicUserKey {
        
        print("Logging out")
        logoutOfAllRealmUsers()
        configUser = configGuest
        
    } else {
        configUser = Realm.Configuration(syncConfiguration: SyncConfiguration(user: SyncUser.current!,
                                                                              realmURL: URL(string: digitalOceamUserRealm)!))
    }
    
}

func logoutOfAllRealmUsers() {
    for user in SyncUser.all {
        debugPrint("user: \(user.key) - \(user.value)")
        user.value.logOut()
    }
}

func seedNewUserAccount(name: String = "Jacob Keith",
                        imageURL: String = "https://lorempixel.com/400/400/cats/",
                        heepID: Int = randomNumber(inRange: 1...1000000),
                        email: String = "",
                        password: String = "",
                        callback: @escaping () -> Void = {},
                        repair: Bool = false) {
    
    
    let newUser = User()
    
    newUser.heepID = heepID
    newUser.name = name
    newUser.iconURL = imageURL
    newUser.email = email
    newUser.icon = getUserIcon(iconURL: newUser.iconURL)
    
    if repair {
        addNewUserToPublicRealm(newUser: newUser, callback: callback)
    } else {
        registerNewSyncRealm(username: email, password: password, callback: callback, newUser: newUser)
        
    }
    
}

func registerNewSyncRealm(username: String, password: String, callback: @escaping () -> Void = {}, newUser: User = User()) {
    let url = URL(string: digitalOceanHTTP)!
    let userURL = URL(string: digitalOceamUserRealm)!
    
    let registerCredentials =  SyncCredentials.usernamePassword(username: username, password: password, register: true)
    
    SyncUser.logIn(with: registerCredentials,
                   server: url,
                   onCompletion: { user, error in
                    
                    if let userUnwrapped = user {
                        configUser =  Realm.Configuration(syncConfiguration: SyncConfiguration(user: userUnwrapped,
                                                                                               realmURL: userURL))
                        addNewUserToUserRealm(newUser: newUser)
                        
                        if let identity = userUnwrapped.identity {
                            print("NOT NIL: \(identity)")
                            newUser.realmKey = identity
                            addNewUserToPublicRealm(newUser: newUser)
                        } else {
                            print("NEW REALM RETURNED NIL")
                        }
                        
                    } else {
                        print("ERROR: \(String(describing: error))")
                        return
                    }
                    
    })
      
}


func loginToUserRealmSync(username: String, password: String, callback: @escaping () -> Void = {}) {
    
    let url = URL(string: digitalOceanHTTP)!
    let userURL = URL(string: digitalOceamUserRealm)!
    let credentials = SyncCredentials.usernamePassword(username: username,
                                                       password: password,
                                                       register: false)
    
    SyncUser.logIn(with: credentials,
                   server: url,
                   onCompletion: { user, error in
                    if user == nil {
                        print("No User Account Found")
                        callback()
                    } else {
                        
                        configUser =  Realm.Configuration(syncConfiguration: SyncConfiguration(user: user!,
                                                                                               realmURL: userURL))
                        
                        print("Found existing")
                        callback()
                    }
                    
    })
    
}

func setDefaultPermissionToPublic(publicUser: SyncUser) {
    let permission = SyncPermissionValue(realmPath: "/" + publicUserKey + "/userDirectory",
                                         userID: "*",
                                         accessLevel: .write)
    
    print("TRYING PERMISSION")
    publicUser.applyPermission(permission) { error in
        print("ENTER?")
        if let error = error {
            // handle error
            print("PERMISSION UNSUCCESSFUL \(error)")
            return
        }
    }
}

func loginToPublicRealm() {
    let url = URL(string: digitalOceanHTTP)!
    
    let credentials = SyncCredentials.usernamePassword(username: "public@heep.io",
                                                       password: "public",
                                                       register: false)
    
    SyncUser.logIn(with: credentials,
                   server: url,
                   onCompletion: { user, error in
                    if user == nil {
                        print("No User Account Found")
                        
                    } else {
                        
                        let firstUser = User()
                        addNewUserToPublicRealm(newUser: firstUser)
                        print("Found existing: \(user)")
                        setDefaultPermissionToPublic(publicUser: user!)
                        
                    }
                    
    })
}

func addNewUserToPublicRealm(newUser: User, callback: @escaping () -> Void = {}) {
    let realm = try! Realm(configuration: configPublicSync)
    
    try! realm.write {
        realm.add(newUser,
                  update: true)
    }
    
    print("ADDED NEW USER TO PUBLIC REALM")
    let allUsers = realm.objects(User.self)
    
    print("All Users: \(allUsers)")
    callback()
    
}

func addNewUserToUserRealm(newUser: User) {
    let realm = try! Realm(configuration: configUser)
    
    try! realm.write {
        realm.create(User.self,
                     value: ["heepID": newUser.heepID],
                     update: true)
    }
    
}





