/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package ownComponents;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JLabel;

/**
 *
 * @author Peter
 */
public class JFrameAreYouSure extends JFrameGridBag{
    
    public JFrameAreYouSure(ActionListener aL, String s) {
        setInsets(4, 8, 4, 8);
        this.addComp(new JLabel(s), 0, 0, 2, 1, 1, 1);
        
        
        JButton bOK = new JButton("YES!"); 
        bOK.addActionListener(aL);
        bOK.addActionListener(new KillListener());
        addComp(bOK, 0, 1, 1, 1, 1, 1);
        
        JButton bNO = new JButton("Not really"); 
        bNO.addActionListener(new KillListener());
        this.addComp(bNO, 1, 1, 1, 1, 1, 1);
        
        setLocation(200, 200);
        pack(); 
        setVisible(true);
    }
    
    class KillListener implements ActionListener {
        public void actionPerformed(ActionEvent ae) {
            kill();
        }
    }
    
    public void kill() {
        this.dispose();
    }

}
