/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package langunator;

import static langunator.OMode.stmt;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JRadioButton;
import javax.swing.JTextField;
import javax.swing.WindowConstants;

import ownComponents.JFrameGridBag;

/**
 * 
 * @author Peter
 */
public class ChangeFrame extends JFrameGridBag {

	private static final long serialVersionUID = -1604021457368371232L;
	JTextField[] tf$ = new JTextField[4];
	String[] sLabelnames = { "Font 1", "Font 2", "Fontsize 1", "Fontsize 2" };
	JRadioButton[] rb = new JRadioButton[3];
	Pool parPool;
	MainFrame mf;

	ChangeFrame(Pool parPool, MainFrame mf) {
		super("Change " + parPool.getName() + "-Settings");
		this.parPool = parPool;
		this.mf = mf;
		setInsets(2, 6, 2, 6);
		for (int n = 0; n < 4; n++) {
			addComp(new JLabel(" " + sLabelnames[n] + " "), 0, n, 1, 1, 0, 0);
			tf$[n] = new JTextField();
			tf$[n].setPreferredSize(new Dimension(160, 24));
			addComp(tf$[n], 1, n, 1, 1, 1, 0);
		}
		this.addComp(new JLabel("Box Mode Multiplier"), 2, 0, 1, 1, 1, 1);
		final ButtonGroup bg = new ButtonGroup();
		for (int n = 2; n < 5; n++) {
			rb[n - 2] = new JRadioButton("" + n);
			addComp(rb[n - 2], 2, n - 1, 1, 1, 1, 0);
			bg.add(rb[n - 2]);
		}
		tf$[0].setText(parPool.getFontName1());
		tf$[1].setText(parPool.getFontName2());
		tf$[2].setText("" + parPool.getFontSize1());
		tf$[3].setText("" + parPool.getFontSize2());
		rb[parPool.getBoxMlt() - 2].setSelected(true);

		final JButton bApply = new JButton("Apply");
		final JButton bOK = new JButton("OK");
		final JButton bCancel = new JButton("Cancel");

		bCancel.addActionListener(new KillListener());
		bApply.addActionListener(new SQLListener());
		bOK.addActionListener(new SQLListener());
		bOK.addActionListener(new KillListener());

		addComp(bApply, 0, 4, 1, 1, 1, 1);
		addComp(bOK, 1, 4, 1, 1, 1, 1);
		addComp(bCancel, 2, 4, 1, 1, 1, 1);

		this.setLocation(200, 200);
		pack();
		setVisible(true);
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

	}

	byte getrbSelection() {
		byte rbSelection = 2;
		for (int n = 2; n < 5; n++) {
			if (rb[n - 2].isSelected()) {
				rbSelection = (byte) n;
			}
		}
		return rbSelection;
	}

	class SQLListener implements ActionListener {
		public void actionPerformed(ActionEvent ae) {
			try {
				stmt.executeUpdate("UPDATE pools set " + "  font1 = '" + tf$[0].getText() + "', font2 = '" + tf$[1].getText()
						+ "', size1 = '" + tf$[2].getText() + "', size2 = '" + tf$[3].getText() + "', boxmlt = '"
						+ getrbSelection() + "' WHERE (id = '" + parPool.getID() + "');");
				mf.loadPool(mf.nowPool.getID());
			} catch (final Exception ex) {
				System.out.println(ex.getMessage());
			}
		}
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
