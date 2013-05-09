/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package langunator;

import static langunator.OMode.stmt;

import java.awt.GridBagConstraints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.ResultSet;
import java.sql.SQLException;

import javax.swing.JButton;
import javax.swing.WindowConstants;

import ownComponents.JFrameAreYouSure;
import ownComponents.JFrameGridBag;

/**
 * 
 * @author Peter
 */
public class DeleteFrame extends JFrameGridBag {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4489884545213171509L;
	GridBagConstraints gbc = new GridBagConstraints();
	MainFrame fr;

	DeleteFrame(MainFrame fr) {
		super("Delete");
		this.fr = fr;
		setInsets(4, 4, 4, 4);

		try {
			final ResultSet rs = stmt.executeQuery("SELECT * FROM pools;");
			byte count = 0;
			while (rs.next()) {
				final String poolTitle = rs.getString("lang1") + "-" + rs.getString("lang2") + "-pool";
				final JButton b = new JButton(poolTitle);
				b.addActionListener(new ActiListener(rs.getString("id"), poolTitle, b));
				addComp(b, 0, count, 1, 1, 0, 0);
				count++;
			}

			this.setLocation(200, 200);
			setVisible(true);
			setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
			pack();

		} catch (final SQLException ex) {
			fr.taInfo.addLine(ex.getMessage());
		}
	}

	private class ActiListener implements ActionListener {
		private final String id;
		JButton b;

		ActiListener(String id, String poolTitle, JButton b) {
			this.id = id;
			this.b = b;
		}

		public void actionPerformed(ActionEvent ae) {
			try {
				final ResultSet rs = stmt.executeQuery("SELECT count(lang1) FROM " + id + ";");
				rs.next();
				if ((rs.getInt(1)) > 20) {
					new JFrameAreYouSure(new KillListener(id, b), b.getText()
							+ " contains more than 20 vocabulary. Are you sure that you want to delete it?");
				} else {
					deletePool(id, b);
				}
				pack();
			} catch (final SQLException ex) {
				System.out.println(ex.getMessage());
			}
		}
	}

	public void deletePool(String id, JButton b) {
		try {
			stmt.executeUpdate("DROP TABLE " + id + ";");
			stmt.executeUpdate("DELETE FROM pools WHERE (id = '" + id + "');");
			fr.taInfo.addLine("Successfully deleted " + b.getText());
			b.setVisible(false);
			pack();
		} catch (final SQLException ex) {
			System.out.println(ex.getMessage());
		}
	}

	class KillListener implements ActionListener {
		private final String id;
		private final JButton b;

		KillListener(String id, JButton b) {
			this.id = id;
			this.b = b;
		}

		public void actionPerformed(ActionEvent ae) {
			deletePool(id, b);
		}
	}

}
