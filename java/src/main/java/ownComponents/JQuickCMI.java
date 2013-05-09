package ownComponents;

import javax.swing.*;

public class JQuickCMI extends JCheckBoxMenuItem{

	public JQuickCMI(String name, String key) {
		super(name);
		setAccelerator(KeyStroke.getKeyStroke(key));
	}
	
	public JQuickCMI(String name, int key, int preKey) {
		super(name);
		setAccelerator(KeyStroke.getKeyStroke(key, preKey));
	}

}
