package ownComponents;

import javax.swing.*;

public class JQuickMI extends JMenuItem{
	
	public JQuickMI(String name, String key) {
		super(name);
		setAccelerator(KeyStroke.getKeyStroke(key));
	}
	
	public JQuickMI(String name, int key, int preKey) {
		super(name);
		setAccelerator(KeyStroke.getKeyStroke(key, preKey));
	}

}
