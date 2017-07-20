//
//  GroupCollectionView.swift
//  mobile_ios
//
//  Created by Jacob Keith on 5/18/17.
//  Copyright © 2017 Heep. All rights reserved.
//

import UIKit
import RealmSwift


class GroupCollectionView: UIViewController, UICollectionViewDelegateFlowLayout, UICollectionViewDataSource {
    var notificationTokenList = [NotificationToken]()
    var collectionView: UICollectionView!
    
    var placeName: String = "placeholder"
    var placeID: Int = 0
    var groups = [Group]()
    
    private let reuseIdentifier = "Cell"
    
    convenience init(placeID: Int, placeName: String) {
        self.init()
        self.placeName = placeName
        self.placeID = placeID
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.title = placeName
        
        self.initRealmNotifications()
        self.setupCollectionView()
        self.setupNavBar()
        
    }
    
    func initRealmNotifications() {
        groups = []
        
        let realm = try! Realm(configuration: configUser)
        let groupsQuery = NSPredicate(format: "placeID = '\(placeID)'")
        
        let groupPerspectives = realm.objects(GroupPerspective.self).filter(groupsQuery).toArray()
        
        for perspective in groupPerspectives {
            
            initGroupNotification(perspective: perspective)
        }
        
    }
    
    func initGroupNotification(perspective: GroupPerspective) {
        let realm = try! Realm(configuration: getGroupConfiguration(path: perspective.realmPath))
        
        guard let group = realm.objects(Group.self).first else {
            print("Did not find a Group Realm for this Group Perspective")
            return
        }
        
        groups.append(group)
        
        let notificationToken = group.addNotificationBlock { changes in

                switch changes {
                case .change:
                    
                    self.reloadView()
                    
                    break
                    
                case .error(let error):
                    
                    fatalError("\(error)")
                    break
                    
                default: break
                }
        }
        
        notificationTokenList.append(notificationToken)
        
    }
    
    func setupNavBar() {
        navigationItem.rightBarButtonItem = UIBarButtonItem(barButtonSystemItem: .add,
                                                            target: self,
                                                            action: #selector(addGroupFromButton))
        
        let search = UIBarButtonItem(title: "Search For Devices",
                                     style: .plain,
                                     target: self,
                                     action: #selector(searchForHeepDevices))
        
        let spacer = UIBarButtonItem(barButtonSystemItem: .flexibleSpace,
                                     target: nil,
                                     action: nil)
        
        let info = UIBarButtonItem(barButtonSystemItem: .organize,
                                   target: self,
                                   action: #selector(openDeviceTable))
        
        self.toolbarItems = [spacer, search, spacer, info]
    }
    
    func setupCollectionView() {
        let layout: UICollectionViewFlowLayout = UICollectionViewFlowLayout()
        layout.sectionInset = UIEdgeInsets(top: 10, left: 10, bottom: 10, right: 10)
        layout.minimumInteritemSpacing = 10
        layout.minimumLineSpacing = 10
        let dimension = (self.view.bounds.width )/2 - layout.sectionInset.left - layout.sectionInset.right + 5
        layout.itemSize = CGSize(width: dimension,
                                 height: dimension)
        
        self.collectionView = UICollectionView(frame: self.view.bounds, collectionViewLayout: layout)
        self.collectionView?.dataSource = self
        self.collectionView?.delegate = self
        self.collectionView?.register(UICollectionViewCell.self, forCellWithReuseIdentifier: reuseIdentifier)
        self.collectionView?.backgroundColor = UIColor.white
        self.view.addSubview(collectionView!)
    }
    
    deinit{
        for token in notificationTokenList {
            token.stop()
        }
    }
    
    
    override func viewWillDisappear(_ animated: Bool) {
        
        for token in notificationTokenList {
            token.stop()
        }
        
        self.title = ""
    }
    
    override func viewWillAppear(_ animated: Bool) {
        self.title = placeName
    }
    

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    func numberOfSections(in collectionView: UICollectionView) -> Int {
        return 1
    }


    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return groups.count
    }

    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: reuseIdentifier, for: indexPath)
        
        cell.backgroundColor = getRandomColor()
        
        let title = UILabel()
        title.text = " " + groups[indexPath.row].name.uppercased() + " "
        title.numberOfLines = 0
        title.sizeToFit()
        title.textColor = .white
        title.font = UIFont.boldSystemFont(ofSize: 30.0)
        
        title.layer.shadowColor = UIColor.lightGray.cgColor
        title.layer.shadowOffset = CGSize(width: 1.0, height: 1.0)
        title.layer.shadowOpacity = 0.5
        title.layer.shadowRadius = 0.25

        title.textAlignment = .center
        title.tag = indexPath.row
        title.isUserInteractionEnabled = true
        title.frame = cell.bounds
        
        let tap = UITapGestureRecognizer(target: self, action: #selector(openGroupView))
        tap.numberOfTapsRequired = 1
        title.addGestureRecognizer(tap)
        
        let thisImageData = groups[indexPath.row].imageData
        
        if (thisImageData == NSData()) {
            
            title.backgroundColor = getRandomColor()
            
        } else {
            title.backgroundColor = .clear
            let image = UIImage(data: thisImageData as Data)
            let imageView = UIImageView(image: image)
            imageView.contentMode = .scaleAspectFit
            imageView.frame = cell.bounds
            cell.addSubview(imageView)
            let overlay = UIView()
            overlay.backgroundColor = UIColor.black
            overlay.alpha = 0.1
            
        }
        
        
        cell.addSubview(title)

        
        
        return cell
    }

}

extension GroupCollectionView {
    
    
    func openGroupView(gesture: UITapGestureRecognizer) {
        print("Open edit Group View")
        print(groups[(gesture.view?.tag)!].name)
        
//        let editRoomView = EditRoomView(groupID: groups[(recognizer.view?.tag)!].id)
//        
//        navigationController?.pushViewController(editRoomView, animated: true)
    }
    
    func openDeviceTable() {
        print("Open Device Table View")
        
//        let seeAllDevicesInPlace = DeviceTableViewController(place: thisPlace)
//        navigationController?.pushViewController(seeAllDevicesInPlace, animated: true)
    }
    
    func addGroupFromButton() {
        //print("this shouldn't be necessary")
        addNewGroupToThisPlace()
    }
    
    func addNewGroupToThisPlace(name: String = "") {
        print("ADD GROUP")
        
    }
    
    func reloadView() {
        
        self.loadView()
        self.viewDidLoad()
    }
    
    func searchForHeepDevices() {
        print("Searching...")
        HeepConnections().SearchForHeepDeviecs()
        
    }
}
