package langunator;

import static langunator.OMode.stmt;

import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.WindowConstants;

import ownComponents.GridBagContainer;
import ownComponents.JFrameGridBag;
import ownComponents.JTextAreaX;
import ownComponents.XTableModel;

public class EditFrame extends JFrameGridBag {

	/**
	 * 
	 */
	private static final long serialVersionUID = -1281450939693943367L;
	GridBagContainer c, cRightMaster;
	JTable table;
	JScrollPane scrollPane;
	JTextField tfLang1 = new JTextField();
	JTextField tfBox = new JTextField("0");
	JTextField tfTime = new JTextField("1");
	JTextField tfSearchNumber = new JTextField();
	JTextField[] tfSearchLang = new JTextField[2];
	JTextField[] tf$ = new JTextField[6];
	JTextArea taLang2 = new JTextArea();
	JTextAreaX taInfo = new JTextAreaX("Info");
	JButton bAdd = new JButton("                        Hinzufügen                        ");
	JButton bChange = new JButton("Ändern");
	JButton bDelete = new JButton("-        Löschen        -");
	JButton bClear = new JButton("Leer");
	JComboBox cbOrder = new JComboBox();

	GridBagConstraints gbc = new GridBagConstraints();

	private final Pool POOL;
	private final String pID;

	String changeID;

	XTableModel model;

	// *** CONSTRUCTOR ***
	EditFrame(Pool parPool, String lang1key, int iEditOrder) {

		setVisible(true);
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

		POOL = parPool;
		pID = POOL.getID();
		setTitle("Vokabeln verwalten: " + POOL.getName());

		this.setInsets(3, 3, 3, 3);

		loadPool();
		// Table soll auf Row-Anklicken reagieren
		table.addMouseListener(new MouseListener() {
			public void mouseExited(MouseEvent e) {
			};

			public void mouseClicked(MouseEvent e) {
				final int row = table.getSelectedRow();
				bChange.setEnabled(true); // Text-Felder sollen
				bDelete.setEnabled(true); // Daten aufnehmen & stuff...
				bChange.setText("Ändern (Zeile: " + (row + 1) + ")");
				bDelete.setText("Löschen (Zeile: " + (row + 1) + ")");
				changeID = table.getValueAt(row, 0).toString();
				tfLang1.setText(table.getValueAt(row, 0).toString());
				taLang2.setText(table.getValueAt(row, 1).toString());
				tfBox.setText(table.getValueAt(row, 2).toString());
				tfTime.setText(table.getValueAt(row, 3).toString());
			};

			public void mouseReleased(MouseEvent e) {
			};

			public void mouseEntered(MouseEvent e) {
			};

			public void mousePressed(MouseEvent e) {
			};
		});

		table.getColumnModel().getColumn(2).setMaxWidth(40);
		table.getColumnModel().getColumn(3).setMaxWidth(48);

		scrollPane = new JScrollPane(table);

		// RIGHT-SIDE COMPONENTS
		// ---------------------
		cRightMaster = new GridBagContainer(2);

		c = new GridBagContainer(0);
		c.setAnchor(GridBagConstraints.EAST);

		c.setFill(GridBagConstraints.HORIZONTAL);

		tfLang1.addKeyListener(new KeyListener() {
			public void keyPressed(KeyEvent k) {
				if (k.getKeyCode() == 9 | k.getKeyCode() == 10) {
					taLang2.requestFocus();
					k.setKeyCode(17);
				}
			}

			public void keyTyped(KeyEvent k) {
			}

			public void keyReleased(KeyEvent k) {
			}
		});

		c.setFill(GridBagConstraints.BOTH);

		taLang2.addKeyListener(new KeyListener() {
			public void keyPressed(KeyEvent k) {
				if (k.getKeyCode() == 9) {
					bAdd.requestFocus();
					k.setKeyCode(17);
				}
			}

			public void keyTyped(KeyEvent k) {
			}

			public void keyReleased(KeyEvent k) {
			}
		});

		c.addComp(new JLabel(table.getColumnName(0)), 0, 0, 1, 1, 1, 0);
		c.addComp(tfLang1, 0, 1, 1, 1, 1, 0);
		c.addComp(new JLabel(table.getColumnName(1)), 0, 2, 1, 1, 1, 0);
		c.addComp(new JScrollPane(taLang2), 0, 3, 1, 1, 1, 1);

		cRightMaster.addComp(c, 0, 0, 2, 1, 0.1, 0.4);

		taLang2.setNextFocusableComponent(bAdd);

		bClear.setFont(new Font("Arial", Font.PLAIN, 10));
		bClear.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				clear();
			}
		});

		c = new GridBagContainer(2);
		c.setFill(GridBagConstraints.HORIZONTAL);
		c.addComp(tfBox, 1, 0, 1, 1, 1, 0.5);
		c.addComp(tfTime, 3, 0, 1, 1, 1, 0.5);
		c.addComp(bClear, 4, 0, 1, 1, 0, 0);
		c.setAnchor(GridBagConstraints.EAST);
		c.addComp(new JLabel("Box-#"), 0, 0, 1, 1, 0, 0.5);
		c.addComp(new JLabel("verbl. Tage"), 2, 0, 1, 1, 0, 0.5);

		cRightMaster.addComp(c, 0, 1, 2, 1, 0.3, 0);

		bAdd.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				add();
			}
		});
		bAdd.addKeyListener(new KeyListener() {
			public void keyReleased(KeyEvent e) {
			}

			public void keyPressed(KeyEvent e) {
				if (e.getKeyCode() == 10) {
					add();
				}
			}

			public void keyTyped(KeyEvent e) {
			}
		});

		bChange.setEnabled(false);
		bChange.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					final Object[] tfc = getTFContents();
					PreparedStatement update = stmt.getConnection().prepareStatement(
							"UPDATE " + pID + " SET lang1=?, lang2=?, box=?, timeleft=? WHERE lang1=?");

					update.setString(1, (String) tfc[0]);
					update.setString(2, (String) tfc[1]);
					update.setInt(3, (Integer) tfc[2]);
					update.setInt(4, (Integer) tfc[3]);
					update.setString(5, changeID);
					update.execute();
					updateTableData();
					clear();

					// updatePool("UPDATE " + pID + " SET lang1=$aa$" + tfc[0] +
					// "$aa$, " + "lang2=$aa$" + tfc[1] + "$aa$, "
					// + "box=" + tfc[2] + ", " + "timeleft=" + tfc[3] + " " +
					// "WHERE lang1=$aa$" + changeID + "$aa$;");
					bDelete.setEnabled(false);
					bChange.setEnabled(false);
					tfLang1.requestFocus();
				} catch (final IntParseException ex) {
					taInfo.addLine(ex.getMessage());
				} catch (SQLException ex) {
					taInfo.addLine(ex.getMessage());
				}
			}
		});

		bDelete.setEnabled(false);
		bDelete.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					PreparedStatement del = stmt.getConnection().prepareStatement("DELETE FROM " + pID + " WHERE lang1=?;");
					updateTableData();
					clear();
					del.execute();
				} catch (SQLException ex) {
					taInfo.addLine(ex.getMessage());
				}
				// updatePool("DELETE FROM " + pID + " WHERE lang1=$aa$" +
				// changeID + "$aa$;");

				bDelete.setEnabled(false);
				bChange.setEnabled(false);
				tfLang1.requestFocus();
			}
		});
		cRightMaster.addComp(bAdd, 0, 2, 2, 1, 0.3, 0);
		cRightMaster.addComp(bChange, 0, 3, 2, 1, 0.3, 0);
		cRightMaster.addComp(bDelete, 0, 4, 2, 1, 0.3, 0);

		for (int i = 0; i < 4; i++) {
			cbOrder.addItem(table.getColumnName(i));
		}
		cbOrder.setSelectedIndex(iEditOrder);
		cbOrder.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				updateTableData();
				MainFrame.setIEditOrder(cbOrder.getSelectedIndex());
			}
		});
		cRightMaster.addComp(new JLabel("Ordne nach: "), 0, 5, 1, 1, 0.1, 0);
		cRightMaster.addComp(cbOrder, 1, 5, 1, 1, 0.1, 0);

		c = new GridBagContainer();
		c.addComp(new JLabel("Suche " + table.getColumnName(0)), 0, 0, 1, 1, 0, 0);
		c.addComp(new JLabel("Suche " + table.getColumnName(1)), 0, 1, 1, 1, 0, 0.5);

		tfSearchLang[0] = new JTextField();
		tfSearchLang[1] = new JTextField();
		c.addComp(tfSearchLang[0], 1, 0, 1, 1, 0.5, 0);
		c.addComp(tfSearchLang[1], 1, 1, 1, 1, 0.5, 0.5);
		tfSearchLang[0].setFont(tfLang1.getFont());
		tfSearchLang[1].setFont(taLang2.getFont());
		tfSearchLang[0].addKeyListener(new SearchListener((byte) 1));
		tfSearchLang[1].addKeyListener(new SearchListener((byte) 2));
		cRightMaster.addComp(c, 0, 6, 2, 1, 0.3, 0.2);

		taInfo.setLineWrap(true);
		taInfo.setMinimumSize(new Dimension(500, 200));
		cRightMaster.addComp(new JScrollPane(taInfo), 0, 7, 2, 1, 0.3, 0.3);

		this.addComp(cRightMaster, 1, 0, 1, 1, 0.1, 1);
		this.addComp(scrollPane, 0, 0, 1, 1, 0.5, 1);

		tfLang1.requestFocus();

		updateTableData();

		setBounds(200, 100, 800, 500);

		if (!lang1key.equals("")) {
			try {
				final ResultSet rs = stmt.executeQuery("SELECT * FROM " + pID + " WHERE (lang1 =" + lang1key + ");");

				if (rs.next()) {
					bChange.setEnabled(true); // Text-Felder sollen
					bDelete.setEnabled(true); // Daten aufnehmen & stuff...
					changeID = rs.getString("lang1");
					tfLang1.setText(rs.getString("lang1"));
					taLang2.setText(rs.getString("lang2"));
					tfBox.setText(rs.getString("box"));
					tfTime.setText(rs.getString("timeleft"));
				}
			} catch (final Exception ex) {
				taInfo.addLine(ex.getMessage());
			}
		}

	}

	// **************
	// CONTRUCTOR END
	// **************

	void add() {
		try {
			final Object[] tfc = getTFContents();
			PreparedStatement prepareStatement = stmt.getConnection().prepareStatement(
					"INSERT INTO " + pID + " (lang1,lang2,box,timeleft) VALUES (?,?,?,?);");
			prepareStatement.setString(1, (String) tfc[0]);
			prepareStatement.setString(2, (String) tfc[1]);
			prepareStatement.setInt(3, (Integer) tfc[2]);
			prepareStatement.setInt(4, (Integer) tfc[3]);
			prepareStatement.execute();
			updateTableData();
			clear();
			tfLang1.requestFocus();
		} catch (final IntParseException ex) {
			taInfo.addLine(ex.getMessage());
		} catch (SQLException e) {
			taInfo.addLine(e.getMessage());
		}
	}

	void clear() { // Hilfs-Methode
		taLang2.setText(""); // zum CLEARen
		tfBox.setText("0"); // der Edit-Komponenten
		tfTime.setText("1");
		tfLang1.setText("");
	}

	Object[] getTFContents() throws IntParseException {
		final Object[] back = new Object[4];
		back[0] = tfLang1.getText();
		back[1] = taLang2.getText();
		try {
			back[2] = Integer.parseInt(tfBox.getText());
		} catch (final Exception e) {
			throw new IntParseException("Box-Feld", tfBox.getText());
		}
		try {
			back[3] = Integer.parseInt(tfTime.getText());
		} catch (final Exception e) {
			throw new IntParseException("Time-Feld", tfTime.getText());
		}
		return back;
	}

	void loadPool() {

		model = new XTableModel(new String[] { POOL.getLang1(), // Model f.
																// Table t$
																// erzeugt
				POOL.getLang2(), // Columns-Setzen
				"Box", "Time" });
		table = new JTable(model);
		// updateTableData();
		tfLang1.setFont(POOL.getFont1());
		taLang2.setFont(POOL.getFont2());
	}

	String getOrderName() {
		switch (cbOrder.getSelectedIndex()) {
		case 0:
			return "lang1";
		case 1:
			return "lang2";
		case 2:
			return "box";
		case 3:
			return "timeleft";
		default:
			return "lang1";
		}
	}

	void updateTableData() {
		try {
			final String order = getOrderName();
			final ResultSet rsData = stmt.executeQuery("SELECT * FROM " + pID + " ORDER BY " + order + ";");
			model.setData(rsData);
		} catch (final SQLException ex) {
			taInfo.setText("SQL-PR");
		}
	}

	void updatePool(String sql) {
		try {
			stmt.executeUpdate(sql);
			updateTableData();
			clear();
		} catch (final Exception ex) {
			taInfo.addLine(ex.getMessage());
		}
	}

	class SearchListener implements KeyListener {
		private final byte par;

		private SearchListener(byte par) {
			this.par = par;
		}

		public void keyPressed(KeyEvent e) {
		}

		public void keyReleased(KeyEvent e) {
			try {
				PreparedStatement prepareStatement = stmt.getConnection().prepareStatement(
						"SELECT * FROM " + pID + " WHERE (lang" + par + " LIKE ?)");
				prepareStatement.setString(1, tfSearchLang[par - 1].getText());

				final ResultSet rs = prepareStatement.executeQuery();

				if (rs.next()) {
					bChange.setEnabled(true); // Text-Felder sollen
					bDelete.setEnabled(true); // Daten aufnehmen & stuff...
					bChange.setText("Change");
					bDelete.setText("Delete");
					changeID = rs.getString("lang1");
					tfLang1.setText(rs.getString("lang1"));
					taLang2.setText(rs.getString("lang2"));
					tfBox.setText(rs.getString("box"));
					tfTime.setText(rs.getString("timeleft"));
				}
			} catch (final Exception ex) {
				taInfo.addLine(ex.getMessage());
			}
		}

		public void keyTyped(KeyEvent e) {
		}
	}

}
