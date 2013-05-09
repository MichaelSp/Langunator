package ownComponents;

import javax.swing.JTextArea;


public class JTextAreaX extends JTextArea {
	
	public JTextAreaX() {
		super();
	}
	public JTextAreaX(String s) {
		super(s);
	}
	
	public String[] getTextArray() {
		char[] c = this.getText().toCharArray();
		int test = this.getLineCount();
		System.out.println(test);
		String[] back = new String[this.getLineCount()];
		String help = "";
		int n = 0;
		for (char now:c) {
			if (now==10) {
				back[n++]=help;
				help="";
			}
			else 
				help = help + now;
		}
		if (help.equals("")) 
			back[n]=help;
		System.out.println(back.length);
		return back;
		
	}
	
	public void addLine(String s) {
		if (this.getText().equals(""))
			this.setText(s);
		else 
			this.setText(getText()+ (char) 10 + s);
	}

}
