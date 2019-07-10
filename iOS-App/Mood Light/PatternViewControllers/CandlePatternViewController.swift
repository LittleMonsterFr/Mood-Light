import UIKit

class CandlePatternViewController: UIViewController, UITextFieldDelegate, UIViewControllerId {
    var textFieldDict = [Int : UITextField]()
    var sliderDict = [Int : UISlider]()
    var textFieldInputCancelled = false
    
    var connect: ChirpConnect?
    var id: Int = 0
    
    @IBAction func sendButton(_ sender: UIBarButtonItem) {
        var data = Data()
        data.append(UInt8(id))
        data.append(UInt8(255))
        for slider in sliders{
            data.append(UInt8(slider.value))
        }
        connect?.send(data)
    }
    
    @IBOutlet var fields: [UITextField]!
        {
        didSet {
            for field in fields {
                field.addDoneCancelToolbar(tag: field.tag,
                                           onDone: (target: self, action: #selector(doneButtonTapped)),
                                           onCancel :(target: self, action: #selector(cancelButtonTapped)))
                field.delegate = self
            }
        }
    }
    
    @IBOutlet var sliders: [UISlider]!
    
    @IBAction func SliderChange(_ sender: UISlider) {
        let tag: Int = sender.tag
        let field: UITextField = fields[tag]
        field.text = String(Int(sender.value))
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        if sliders.count != fields.count {
            return
        }
        
        // Ensure the tag are synchronized and ordered
        for index in 0..<sliders.count {
            sliders[index].tag = index
            fields[index].tag = index
        }
        
        // Get the connect instance
        let appDelegate = UIApplication.shared.delegate as! AppDelegate
        connect = appDelegate.connect
    }
    
    override func viewDidAppear(_ animated: Bool) {
        if sliders.count != fields.count {
            let alertController = UIAlertController(title: "Alert", message: "Slider list size different than field list size.", preferredStyle: .alert)
            self.present(alertController, animated: true, completion: nil)
        }
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @objc func doneButtonTapped(sender: UIBarButtonItem)
    {
        let tag: Int = sender.tag
        let field: UITextField = fields[tag]
        let slider : UISlider = sliders[tag]
        slider.value = Float(Int(field.text!)!)
        if slider.value == slider.maximumValue {
            field.text = String(Int(slider.maximumValue))
        }
        field.resignFirstResponder()
    }
    
    @objc func cancelButtonTapped(sender: UIBarButtonItem)
    {
        let tag: Int = sender.tag
        let field: UITextField = fields[tag]
        let slider: UISlider = sliders[tag]
        field.text = String(Int(slider.value))
        textFieldInputCancelled = true
        field.resignFirstResponder()
    }
    
    func textFieldDidEndEditing(_ textField: UITextField) {
        if !textFieldInputCancelled {
            let tag: Int = textField.tag
            let slider: UISlider = sliders[tag]
            slider.value = Float(textField.text!)!
            textFieldInputCancelled = false
        }
    }
}

