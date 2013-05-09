package ownComponents;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

public class GridBagContainer extends Container{

	private GridBagLayout gbl = new GridBagLayout();
    private GridBagConstraints gbc = new GridBagConstraints(); 
	
	public GridBagContainer() {	
		super();
	    construction();
	}
	public GridBagContainer(int insets) {
		super();
		construction();
		setInsets(insets, insets, insets, insets);
	}
	void construction() {
	    this.setLayout(gbl); 
	    gbc.anchor = GridBagConstraints.CENTER;
	}
	
	
	public void setAnchor(int i) {
		gbc.anchor = i;
	}
	
	public void setInsets(int iTop, int iLeft, int iBottom, int iRight) {
		gbc.insets = new Insets(iTop, iLeft, iBottom, iRight);
	}
	public void setColor(Color c) {
		setBackground(c);
	}
	public void setFill(int i) {
		gbc.fill = i;
	}
	
	public void addComp( 
            Component c, 
            int x, int y, 
            int width, int height, 
            double weightx, double weighty ) 
    { 
	    gbc.fill = GridBagConstraints.BOTH; 
	    gbc.gridx = x; gbc.gridy = y; 
	    gbc.gridwidth = width; gbc.gridheight = height; 
	    gbc.weightx = weightx; gbc.weighty = weighty; 
	    gbl.setConstraints( c, gbc ); 
	    add( c );
	}
	
	
}
