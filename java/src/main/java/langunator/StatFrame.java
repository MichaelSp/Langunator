package langunator;

import static langunator.OMode.stmt;

import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.WindowConstants;

public class StatFrame extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = 6193665498049174732L;
	JButton[] bQuant = new JButton[12];
	JButton[] bName = new JButton[12];
	JLabel[] lCount = new JLabel[12];
	Font f = new Font("Arial", Font.BOLD, 14);
	JComboBox cbPools = new JComboBox();
	JLabel l1 = new JLabel();
	private final String sAV = "Average per Day: ";
	List<String> pools = new ArrayList<String>();
	Color[] cBackground = new Color[10];

	// ***** CONSTRUCTOR *****
	StatFrame() {
		setLocation(200, 100);
		setSize(736, 576);
		setVisible(true);
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		setLayout(null);
		this.setTitle("Statistics");

		for (int i = 0; i < 12; i++) {
			bName[i] = new JButton("" + i);
			bName[i].setBounds(i * 60, 520, 60, 20);
			bName[i].setFont(f);
			bName[i].setForeground(new Color(200, 200, 100));
			bName[i].setBackground(new Color(255 - i * 20, 50, 5 + i * 20));
			add(bName[i]);
			bQuant[i] = new JButton();
			bQuant[i].setBounds(i * 60, 520, 60, 0);
			add(bQuant[i]);
			lCount[i] = new JLabel();
			lCount[i].setSize(40, 12);
			add(lCount[i]);
		}

		l1.setText(sAV);
		l1.setBounds(208, 0, 150, 20);
		add(l1);

		cbPools.setBounds(0, 0, 200, 20);

		try {
			final ResultSet $ = stmt.executeQuery("SELECT id FROM pools;");
			while ($.next()) {
				pools.add($.getString(1));
				cbPools.addItem($.getString(1));
			}
			cbPools.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					init(cbPools.getSelectedItem().toString());
				}
			});
			add(cbPools);
		} catch (final Exception ex) {
			System.out.println("Exc. @ Stat.: " + ex.getMessage());
		}
	} // END Standard-Constructor

	StatFrame(String inPool) {
		this();
		init(inPool);
		cbPools.setSelectedIndex(pools.indexOf(inPool));
	}

	private void init(String pool) {
		try {
			ResultSet $ = stmt.executeQuery("SELECT (boxmlt) FROM pools WHERE (id = '" + pool + "');");
			$.next();
			final int boxmlt = $.getInt("boxmlt");
			$ = stmt.executeQuery("SELECT box FROM " + pool + ";");
			final int[] count = new int[15];
			while ($.next()) {
				count[$.getInt(1)]++;
			}
			int max = 0;
			double av = 0;
			for (int i = 0; i < 12; i++) {
				av = av + (count[i] / Math.pow(boxmlt, i));
				if (count[i] > max)
					max = count[i];
			}
			l1.setText(sAV + (Math.round(av * 10.0)) / 10.0);
			for (int i = 0; i < 12; i++) {
				if ((count[i] * 500 / max) < 16) {
					lCount[i].setText("" + count[i]);
					lCount[i].setLocation(i * 60 + 26, 506 - count[i] * 500 / max);
					lCount[i].setVisible(true);
					bQuant[i].setText("");
				} else {
					lCount[i].setVisible(false);
					bQuant[i].setText("" + count[i]);
				}
				if (count[i] == 0) {
					lCount[i].setVisible(false);
				}
				bQuant[i].setBounds(i * 60, 520 - count[i] * 500 / max, 60, count[i] * 500 / max);
			} // END for i 1 to 11
		} // END try
		catch (final SQLException ex) {
			System.out.println("SQLex. " + ex.getMessage());
		} catch (final Exception ex) {
			System.out.println(ex.getMessage());
		}
	} // END init

}
