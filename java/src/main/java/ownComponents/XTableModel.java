package ownComponents;

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;

import javax.swing.table.DefaultTableModel;

public class XTableModel extends DefaultTableModel {

	public void clear() {
		for (int n = 0; n < this.getRowCount();) {
			this.removeRow(n);
		}
	}

	public void addData(ResultSet rs) {
		try {
			ResultSetMetaData rsmd = rs.getMetaData();
			while (rs.next()) {
				Object[] obj = new Object[rsmd.getColumnCount()];
				for (int i = 0; i < rsmd.getColumnCount(); i++) {
					obj[i] = rs.getObject(i + 1);
				}
				addRow(obj);
			}
		} catch (SQLException ex) {
			System.out.println("ACHTUNG!! SQL-Exception in XTableModel!!");
			System.out.println(ex.getMessage());
		}
	}

	public void setData(ResultSet rs) {
		clear();
		addData(rs);
	}

	public XTableModel(Object[][] data, String[] cols) {
		super(data, cols);
	}

	public XTableModel(String[] cols) {
		super(new Object[][] {}, cols);
	}

}
