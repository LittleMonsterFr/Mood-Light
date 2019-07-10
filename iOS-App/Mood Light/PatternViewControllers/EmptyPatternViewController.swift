//
//  ViewController.swift
//  Matrix
//
//  Created by Damien Laidin on 13/04/2018.
//  Copyright © 2018 Damien Laidin. All rights reserved.
//

import UIKit

class EmptyPatternViewController: UIViewController, UITextFieldDelegate, UIViewControllerId {
    var connect: ChirpConnect?
    var id: Int = 0
    
    @IBAction func sendButton(_ sender: UIBarButtonItem) {
        var data = Data()
        data.append(UInt8(id))
        connect?.send(data)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Get the connect instance
        let appDelegate = UIApplication.shared.delegate as! AppDelegate
        connect = appDelegate.connect
    }
    
    override func viewDidAppear(_ animated: Bool) {}
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
}

