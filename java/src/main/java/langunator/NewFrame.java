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

public class NewFrame extends JFrameGridBag {

	private static final long serialVersionUID = 1937527970228561037L;
	JTextField[] tf$ = new JTextField[6];
	String[] sLabelnames = { "Sprache 1", "Sprache 2", "Font 1", "Font 2", "Fontgröße 1", "Fontgröße 2" };
	JButton bOK, bAbort;
	String poolName;

	NewFrame(MainFrame fr) {

		super("Neuen Pool erstellen");
		final MainFrame f = fr;
		this.setInsets(1, 4, 1, 4);
		for (int n = 0; n < 6; n++) {
			addComp(new JLabel(" " + sLabelnames[n] + " "), 0, n, 1, 1, 0, 0);
			String s = "";
			if ((n > 1) & (n < 4)) {
				s = "Arial";
			}
			if (n > 3) {
				s = "12";
			}
			tf$[n] = new JTextField(s);
			tf$[n].setPreferredSize(new Dimension(160, 24));
			addComp(tf$[n], 1, n, 1, 1, 1, 0);
		}
		this.addComp(new JLabel("Box-Modus-Multiplikator"), 2, 0, 1, 1, 1, 1);
		final ButtonGroup bg = new ButtonGroup();
		final JRadioButton[] rb = new JRadioButton[3];
		for (int n = 2; n < 5; n++) {
			rb[n - 2] = new JRadioButton("" + n);
			addComp(rb[n - 2], 2, n - 1, 1, 1, 1, 0);
			bg.add(rb[n - 2]);
		}
		rb[0].setSelected(true);
		bOK = new JButton("~ OK ~");
		bOK.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent a) {
				try {
					poolName = tf$[0].getText() + tf$[1].getText() + "pool";

					byte rbSelection = 2;
					for (int n = 2; n < 5; n++) {
						if (rb[n - 2].isSelected()) {
							rbSelection = (byte) n;
						}
					}
					// Neuer Eintrag (Row) in pools-Table
					stmt.executeUpdate("INSERT INTO pools "
							+ "(id, lang1, lang2, font1, font2, size1, size2, boxmlt, previous) VALUES  ('"
							+ poolName
							+ "', "
							+ "'"
							+ tf$[0].getText()
							+ "', "
							+ "'"
							+ tf$[1].getText()
							+ "', "
							+ "'"
							+ tf$[2].getText()
							+ "', "
							+ "'"
							+ tf$[3].getText()
							+ "', "
							+ "'"
							+ tf$[4].getText()
							+ "', "
							+ "'"
							+ tf$[5].getText() + "',  '" + rbSelection + "', " + util.getDay() + ");");

					// Neuer Table
					final String s = "CREATE TABLE " + poolName + "( lang1 text, lang2 text, box int2,"
							+ "timeleft int2,  CONSTRAINT " + poolName + "_pkey PRIMARY KEY (lang1)  );";
					stmt.executeUpdate(s);
					f.taInfo.addLine("Successfully created " + poolName);
					f.loadPool(poolName);
					killFrame();
				} catch (final Exception ex) {
					System.out.println(ex.getMessage());
				}

			} // END actionPerformed
		});
		addComp(bOK, 0, 6, 2, 1, 1, 1);

		bAbort = new JButton("Abbrechen");
		bAbort.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				killFrame();
			}
		});
		this.addComp(bAbort, 2, 6, 1, 1, 1, 1);

		this.setLocation(200, 200);
		setVisible(true);
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		pack();
	}

	private void killFrame() {
		this.dispose();
	}

}
