package ownComponents;

import java.awt.ComponentOrientation;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JTextField;

public class JTextFieldX extends JTextField implements KeyListener{
	
	private String text;
	
	JTextFieldX() {
		super();
	}
	JTextFieldX(String s) {
		super(s);
		text = s;
	}
	
	void rightToLeft(boolean rtl) {
		this.setComponentOrientation(rtl?ComponentOrientation.RIGHT_TO_LEFT:ComponentOrientation.LEFT_TO_RIGHT);	
	}
	
	public void keyPressed(KeyEvent e) {}
	public void keyReleased(KeyEvent e) {}
	public void keyTyped(KeyEvent e) {
		text = this.getText();
		setText(text);
	}
	
}
