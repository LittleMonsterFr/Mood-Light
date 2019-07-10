//
//  PatternTableViewController.swift
//  Matrix
//
//  Created by Damien Laidin on 18/04/2018.
//  Copyright © 2018 Damien Laidin. All rights reserved.
//

import UIKit
import os.log

class PatternTableViewController: UITableViewController {
    
    var patterns = [(id: Int, name: String)]()
    var selectedPatternId : Int = 0
    var controllers = [Int : UIViewControllerId]()
    var connect: ChirpConnect?
    
    private func loadPatterns() {
        let mainBundle = Bundle.main.url(forResource: "patterns", withExtension: "json")
        guard let data = try? Data(contentsOf: mainBundle!) else { return }
        let json = try? JSONSerialization.jsonObject(with: data, options: [])
        if let root = json as? [String: Any] {
            for pattern in root {
                let name = pattern.key
                let values = pattern.value as? [String: Any]
                for (key, value) in values! {
                    if key == "id" {
                        patterns += [(value as! Int, name)]
                    }
                }
            }
        }
        patterns.sort(by: { $0.id < $1.id })
    }
    
    @IBAction func onPowerOff(_ sender: UIBarButtonItem) {
        var data = Data()
        for i in 0...7 {
            data.append(UInt8(i))
        }
        connect?.send(data)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        loadPatterns()
        
        // Get the connect instance
        let appDelegate = UIApplication.shared.delegate as! AppDelegate
        connect = appDelegate.connect
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    // MARK: - Table view data source
    
    override func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return patterns.count
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cellIdentifier = "PatternTableViewCell"
        
        guard let cell = tableView.dequeueReusableCell(withIdentifier: cellIdentifier, for: indexPath) as? PatternTableViewCell
            else {
                fatalError("The dequeued cell is not an instance of PatternTableViewCell.")
        }
        
        let pattern = patterns[indexPath.row]
        cell.cellLabel.text = String(pattern.id) + " - " + pattern.name
        
        return cell
    }
    
    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        selectedPatternId = patterns[indexPath.row].id
        let patternName = patterns[indexPath.row].name
        self.performSegue(withIdentifier: patternName, sender: self)
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        super.prepare(for: segue, sender: sender)
        let navControler: UINavigationController = segue.destination as! UINavigationController

        if let controller = controllers[selectedPatternId] {
            navControler.setViewControllers([controller as! UIViewController], animated: true)
        }
        else {
            var controller : UIViewControllerId = navControler.viewControllers.first as! UIViewControllerId
            controller.id = selectedPatternId
            (controller as! UIViewController).navigationItem.title = patterns[selectedPatternId - patterns[0].id].name
            controllers[selectedPatternId] = controller
        }
    }
    
    @IBAction func unwindToPatternList(sender: UIStoryboardSegue) {
        
    }
}
