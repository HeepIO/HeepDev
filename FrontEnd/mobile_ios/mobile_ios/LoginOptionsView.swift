//
//  LoginOptionsView.swift
//  mobile_ios
//
//  Created by Jacob on 6/7/17.
//  Copyright © 2017 Heep. All rights reserved.
//

import UIKit
import RealmSwift
import FacebookLogin
import FacebookCore

class LoginOptionsView: UIViewController, UICollectionViewDataSource, UICollectionViewDelegate {
    
    var collectionView: UICollectionView!
    var prevView = AccountView()
    private let reuseIdentifier = "Cell"
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.view.backgroundColor = UIColor.black.withAlphaComponent(0.5)
        let tap = UITapGestureRecognizer(target: self, action: #selector(exitModalView))
        self.view.addGestureRecognizer(tap)
        
        let subview = UIView()
        subview.frame = CGRect(x: self.view.frame.width/6,
                                 y: self.view.frame.height/3,
                                 width: 2 * self.view.bounds.width/3,
                                 height: self.view.bounds.height/3)
        subview.addGestureRecognizer(UITapGestureRecognizer(target: nil, action: nil))
        subview.layer.cornerRadius = 5
        subview.clipsToBounds = true
        
        subview.addSubview(setupCollectionView(frame: subview.frame))
        self.view.addSubview(subview)
        self.title = "Login Options"
    }
    
    func setupCollectionView(frame: CGRect) -> UICollectionView {
        
        let layout: UICollectionViewFlowLayout = UICollectionViewFlowLayout()
        layout.sectionInset = UIEdgeInsets(top: 10, left: 10, bottom: 10, right: 10)
        layout.minimumInteritemSpacing = 10
        layout.minimumLineSpacing = 10
        let dimension = (frame.width )/2 - layout.sectionInset.left - layout.sectionInset.right + 5
        layout.itemSize = CGSize(width: dimension,
                                 height: dimension)
        
        self.collectionView = UICollectionView(frame: CGRect(x: 0, y: 0, width: frame.width, height: frame.height), collectionViewLayout: layout)
        self.collectionView?.dataSource = self
        self.collectionView?.delegate = self
        self.collectionView?.register(UICollectionViewCell.self, forCellWithReuseIdentifier: reuseIdentifier)
        self.collectionView?.backgroundColor = UIColor.white
        return collectionView!
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    func numberOfSections(in collectionView: UICollectionView) -> Int {
        return 1
    }
    
    
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return 2
    }
    
    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: reuseIdentifier, for: indexPath)
        
        cell.backgroundColor = .white
        cell.tag = indexPath.row
        cell.isUserInteractionEnabled = true
        
        
        var thisCell = UIView()
        var tap = UITapGestureRecognizer(target: self, action: #selector(loginWithEmail))
        
        switch indexPath.row {
        case 1:
            thisCell = renderLoginOption(frame: cell.bounds,
                                         text: " facebook ",
                                         icon: #imageLiteral(resourceName: "FB-f-Logo__blue_1024"))
            
            tap = UITapGestureRecognizer(target: self, action: #selector(loginWithFacebook))
            
        default:
            thisCell = renderLoginOption(frame: cell.bounds,
                                         text: " email ",
                                         icon: #imageLiteral(resourceName: "mail"))
            
                    }
        
        cell.addSubview(thisCell)
        cell.addGestureRecognizer(tap)
        
        return cell
    }
    
    func renderLoginOption(frame: CGRect, text: String, icon: UIImage = UIImage()) -> UIView {
        let customView = UIView()
        customView.frame = frame
        customView.layer.borderColor = UIColor.gray.cgColor
        customView.layer.borderWidth = 0
        customView.layer.cornerRadius = 5
        
        let title = UILabel()
        title.text = text
        title.adjustsFontSizeToFitWidth = true
        title.textColor = .gray
        title.font = UIFont.boldSystemFont(ofSize: 30.0)
        
        title.layer.shadowColor = UIColor.lightGray.cgColor
        title.layer.shadowOffset = CGSize(width: 1.0, height: 1.0)
        title.layer.shadowOpacity = 0.3
        title.layer.shadowRadius = 0.25
        
        title.textAlignment = .center
        title.frame = CGRect(x: 0,
                             y: customView.frame.maxY - 40,
                             width: customView.frame.width,
                             height: 35)
        
        //let optionIconView = UIImageView()
        
        let optionIconView = fitAndCenterImageInView(frame: CGRect(x: 0,
                                                                   y: customView.frame.minY + 20,
                                                                   width: customView.frame.width,
                                                                   height: customView.frame.maxY - 60),
                                                     image: icon)
        
        //optionIconView.contentMode = .scaleAspectFill
        
        customView.addSubview(optionIconView)
        customView.addSubview(title)
        return customView
    }
    
}

extension LoginOptionsView {
    func loginWithEmail() {
        print("Login with email...")
    }
}

extension LoginOptionsView : LoginButtonDelegate {
    
    func loginWithFacebook() {
        let fbLoginManager : LoginManager = LoginManager()
        fbLoginManager.logIn([.publicProfile, .email], viewController: self) { result in
            print(result)
            self.facebookLogin()
        }
        fbLoginManager.logOut()
    }
    
    
    func loginButtonDidCompleteLogin(_ loginButton: LoginButton, result: LoginResult) {
        print("login button did complete")
        switch result {
        case .failed(let error):
            print(error)
        case .cancelled:
            print("Cancelled")
        case .success(_,_,_):
            print("Logged In")
            facebookLogin()
        }
    }
    
    func position(for bar: UIBarPositioning) -> UIBarPosition{
        return .topAttached;
    }
    
    func loginButtonDidLogOut(_ loginButton: LoginButton) {
        print("Logged Out")
    }
    
    func facebookLogin() {
        if let accessToken = AccessToken.current {
            //print(UserDefaults.standard.dictionaryRepresentation())
            let params = ["fields":"name,email,picture"]
            let graphRequest = GraphRequest(graphPath: "me", parameters: params)
            graphRequest.start { (urlResponse, requestResult) in
                switch requestResult {
                case .failed(let error):
                    print(error)
                case .success(let graphResponse):
                    if let responseDictionary = graphResponse.dictionaryValue {
                        print(responseDictionary)
                        UserDefaults.standard.set(responseDictionary, forKey: "userInfo")
                        let name = responseDictionary["name"] as! String
                        let pic = responseDictionary["picture"] as! NSDictionary
                        let data = pic["data"] as! NSDictionary
                        let url = data["url"] as! String
                        let id = responseDictionary["id"] as! String
                        self.seedNewUserFromFacebook(name: name, imageURL: url, id: id)
                    }
                }
            }
            
            /*
             let graphFriendRequest = GraphRequest(graphPath: "/103528713584037/friends", parameters: params)
             graphFriendRequest.start { (urlResponse, requestResult) in
             switch requestResult {
             case .failed(let error):
             print(error)
             case .success(let graphResponse):
             if let responseDictionary = graphResponse.dictionaryValue {
             print(responseDictionary)
             //UserDefaults.standard.set(responseDictionary, forKey: "userInfo")
             
             }
             }
             }
             
             */
            
        } else {
        }
    }
    
    func seedNewUserFromFacebook(name: String, imageURL: String, id: String) {
        let realm = try! Realm(configuration: configApp)
        let app = realm.object(ofType: App.self, forPrimaryKey: 0)
        let newUser = User()
        //print(actualInfo)
        
        newUser.userID = Int(id)!
        newUser.facebookID = Int(id)!
        newUser.name = name
        newUser.iconURL = imageURL
        print(newUser)
        
        try! realm.write {
            app?.activeUser = Int(id)!
            realm.add(newUser,
                      update: true)
        }
        
        let iconData = getUserIcon(iconURL: newUser.iconURL)
        
        try! realm.write {
            
            newUser.icon = iconData
        }
        print("After getting image \(newUser)")
        
        loginToUserRealm(user: newUser.userID)
    }
    
    func getUserIcon(iconURL: String) -> NSData {
        let url = URL(string: iconURL)
        let data = try? Data(contentsOf: url!)
        
        return data! as NSData
    }
    
    func loginToUserRealm(user: Int) {
        let realmApp = try! Realm(configuration: configApp)
        let app = realmApp.object(ofType: App.self, forPrimaryKey: 0)
        
        try! realmApp.write {
            app?.activeUser = user
        }
        
        configUser.fileURL = configUser.fileURL!.deletingLastPathComponent().appendingPathComponent("\(String(describing: user)).realm")
        
        exitModalView()
    }
    
    func exitModalView() {
        print("exiting")
        self.dismiss(animated: true, completion: {
            self.prevView.exitModalView()
        })
    }
}


