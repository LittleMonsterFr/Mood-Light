import Foundation
import UIKit

extension UITextField {
    
    func addDoneCancelToolbar(tag: Int,
                              onDone: (target: Any, action: Selector)? = nil,
                              onCancel: (target: Any, action: Selector)? = nil)
    {
        let toolbar: UIToolbar = UIToolbar()
        toolbar.barStyle = .default
        
        let onCancel = onCancel ?? (target: self, action: #selector(cancelButtonTapped))
        let onDone = onDone ?? (target: self, action: #selector(doneButtonTapped))
        
        let cancel: UIBarButtonItem = UIBarButtonItem(title: "Cancel",
                                                      style: .done,
                                                      target: onCancel.target,
                                                      action: onCancel.action)
        cancel.tag = tag
        
        let flexSpace = UIBarButtonItem(barButtonSystemItem: .flexibleSpace,
                                        target: nil,
                                        action: nil)
        
        let done: UIBarButtonItem = UIBarButtonItem(title: "Done",
                                                    style: .done,
                                                    target: onDone.target,
                                                    action: onDone.action)
        done.tag = tag
        
        let items = [cancel, flexSpace, done]
        toolbar.items = items
        toolbar.sizeToFit()
        
        self.inputAccessoryView = toolbar
    }
    
    @objc func doneButtonTapped()
    {
        print("In done button tapped")
        self.resignFirstResponder()
    }
    
    @objc func cancelButtonTapped()
    {
        print("In cancel button tapped")
        self.resignFirstResponder()
    }
}
