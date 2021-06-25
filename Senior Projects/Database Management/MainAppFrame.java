/* AUTHORS: Hunter Ward and Matthew Schwartz
 * FILE: project_GUI.java
 * DESC: This file implements a graphical interface for a program and allows
 * for interaction with a database. This program can be used to monitor
 * various products, see basic metrics, and modify/add any products to their store.
 * Think of this as a "back-end" application for use by only employees.
 * 
 * This GUI was developed with no previous GUI experience, so some things
 * here may be unconventional. However, we deliberately chose to do this
 * for our project so that we could also learn something new, and because
 * when it came time to make a decision on the GUI, we felt our project itself
 * was not as complex or in-depth as it could be, so we wanted to make a 
 * GUI that could help redeem ourselves a bit. This was developed and tested
 * using Eclipse and the WindowBuilder plugin, which allowed for "drag and drop"
 * functionality for adding UI elements. As the GUI became more complex, we
 * were able to just add those elements with code, and small tweaks were required
 * that the plugin could not support.
 */


package newproject;

import java.awt.BorderLayout;
import java.awt.EventQueue;
import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableModel;

import java.awt.Font;
import java.awt.event.ActionListener;
import java.awt.event.ComponentListener;
import java.sql.*;
import java.util.Vector;
import java.awt.event.ActionEvent;

public class project_GUI extends JFrame { 
	// global vars
	// these are added so actionListeners are easier to implement
	// that way, the same resultset can be built/deleted to easily, allowing to refresh quicker
	// build and remove tables
	private Queries connection = new Queries(); // global connection to DB
	
	private ResultSet allProducts;
	private ResultSet highestMargin_products;
	private ResultSet mostSold_products;
	private ResultSet needsRestocked_products;
	private ResultSet order_contents;
	private ResultSet orders_RS;
	private boolean initialization = true;
	
	private JPanel contentPane;
	private final JRadioButton EmployeeButton = new JRadioButton("Modify products/orders");
	private final JRadioButton Incoming_button = new JRadioButton("See incoming orders");
	private final ButtonGroup empCust_btnGroup = new ButtonGroup();
	private final JRadioButton MetricButton = new JRadioButton("Metric View");
	private final ButtonGroup ship_btnGroup = new ButtonGroup();
	private final JRadioButton shipped_btn = new JRadioButton("Show shipped orders");
	private final JRadioButton unshipped_btn = new JRadioButton("Show unshipped orders");
	private final JButton addProductButton = new JButton("Add Product");
	private final JButton rmv_button = new JButton("Remove");
	private final JButton fetchOrderContents_button = new JButton("Fetch Order Contents");
	private final JButton update_button = new JButton("Update Product");
	
	private final JLabel MetricsLabel = new JLabel("Metrics:");
	private final JLabel MostSoldLabel = new JLabel("Most sold product:");
	private final JLabel MostProfitLabel = new JLabel("Highest profit margin:");
	private final JLabel RestockLabel = new JLabel("Needs restocked:");
	private final JLabel AddProductLabel = new JLabel("Add product");
	private final JLabel ID_Label = new JLabel("ID:");
	private final JLabel NameLabel = new JLabel("Name:");
	private final JLabel CategoryLabel = new JLabel("Category:");
	private final JLabel WholesaleLabel = new JLabel("Wholesale:");
	private final JLabel RetailPriceLabel = new JLabel("Retail:");
	private final JLabel ProductListLabel = new JLabel("Product List:");
	private final JLabel WeightLabel = new JLabel("Weight (kg):");
	private final JLabel QuantityLabel = new JLabel("Quantity:");
	private final JLabel RestockNotificationLabel = new JLabel("Restock Notification:");
	private final JLabel RemoveProdLabel = new JLabel("Remove Product");
	private final JLabel rmvID_label = new JLabel("Item ID:");
	private final JLabel fetchOrder_lbl = new JLabel("Enter an order ID:");
	private final JLabel outgoingContents_lbl = new JLabel("Contents for order #:");
	private final JLabel lblNewLabel = new JLabel("Total Sold (Update Only):");
	
	private final JTextField Name_textField = new JTextField();
	private final JTextField Category_textField = new JTextField();
	private final JTextField Wholesale_textField = new JTextField();
	private final JTextField Retail_textField =  new JTextField();
	private final JTextField Restock_textField = new JTextField();
	private final JTextField Quantity_textField = new JTextField();
	private final JTextField Weight_textField = new JTextField();
	private final JTextField ID_textField = new JTextField();
	private final JTextField orderID_textField = new JTextField();
	private final JTextField rmv_textField = new JTextField();
	private final JTextField totalSold_textField = new JTextField();
	
	private final DefaultTableModel product_tableModel = new DefaultTableModel(new String[]{"ID", "Name", "Category", "Weight (kg)", "Wholesale $", "Retail $", "Amount left", "Restock Threshold", "Total sold"}, 0);
	private final DefaultTableModel margin_tableModel = new DefaultTableModel(new String[]{"ID", "Name", "Profit Margin ($)", "Total Sold"}, 0);
	private final DefaultTableModel mostSold_tableModel = new DefaultTableModel(new String[]{"ID", "Name", "Retail $", "Total Sold"}, 0);
	private final DefaultTableModel needsRestocked_tableModel = new DefaultTableModel(new String[]{"ID", "Name", "Restock Threshold:", "Amount left:"}, 0);
	private final DefaultTableModel contents_tableModel = new DefaultTableModel(new String[] {"Product ID:", "Quantity:"},0);
	private final DefaultTableModel orders_tableModel = new DefaultTableModel(new String[] {"Order ID/Supplier:", "Date Placed:", "Shipping ID:"},0);
	
	private final JTable mostSold_table = new JTable(mostSold_tableModel);
	private final JTable highMargin_table = new JTable(margin_tableModel);
	private final JTable restock_table = new JTable(needsRestocked_tableModel);
	private final JTable orderContents_table = new JTable();
	private JTable product_table = new JTable(product_tableModel);
	private final JTable orders_table = new JTable();
	
	private final JScrollPane product_pane = new JScrollPane();
	private final JScrollPane mostSold_pane = new JScrollPane();
	private final JScrollPane margin_pane = new JScrollPane();
	private final JScrollPane restock_pane = new JScrollPane();
	private final JScrollPane outgoingContents_pane = new JScrollPane();
	private final JScrollPane outgoingOrders_pane = new JScrollPane();
	
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					project_GUI frame = new project_GUI();
					frame.start();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	public void start() {
		setVisible(true);
		project_GUI.this.revalidate();
		project_GUI.this.repaint();
		runProgram();
	}
	public project_GUI() {
		initGUI();
	}
	public void initGUI() {
		// this function does all the initialization. This was
		// mostly generated from the window builder plug-in.
		// Having all these functions in here allow for the
		// plug-in to automatically adjust any values we set in the preview window.
		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 1280, 720);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(null);
		addMainButtons();
		orderID_textField.setBounds(340, 520, 86, 20);
		orderID_textField.setColumns(10);
		totalSold_textField.setBounds(429, 137, 86, 20);
		totalSold_textField.setColumns(10);
		rmv_textField.setBounds(653, 88, 86, 20);
		rmv_textField.setColumns(10);
		
		Retail_textField.setBounds(149, 162, 86, 20);
		Retail_textField.setColumns(10);
		Wholesale_textField.setBounds(149, 137, 86, 20);
		Wholesale_textField.setColumns(10);
		Category_textField.setBounds(149, 112, 86, 20);
		Category_textField.setColumns(10);
		Name_textField.setBounds(149, 88, 86, 20);
		Name_textField.setColumns(10);
		ID_textField.setBounds(149, 65, 86, 20);
		ID_textField.setColumns(10);
		
		contentPane.add(MetricButton);
		AddProductLabel.setFont(new Font("Tahoma", Font.BOLD, 13));
		AddProductLabel.setBounds(27, 37, 88, 14);
		contentPane.add(AddProductLabel);
		ID_Label.setBounds(27, 65, 120, 14);
		contentPane.add(ID_Label);
		NameLabel.setBounds(27, 87, 109, 14);
		contentPane.add(NameLabel);
		CategoryLabel.setBounds(27, 112, 109, 14);
		contentPane.add(CategoryLabel);
		WholesaleLabel.setBounds(27, 137, 120, 14);
		contentPane.add(WholesaleLabel);
		RetailPriceLabel.setBounds(27, 162, 120, 14);
		contentPane.add(RetailPriceLabel);
		
		contentPane.add(ID_textField);
		contentPane.add(Name_textField);
		contentPane.add(Category_textField);
		contentPane.add(Wholesale_textField);
		contentPane.add(Retail_textField);
		
		addProductButton.setBounds(300, 162, 142, 23);
		
		contentPane.add(addProductButton);
		
		// metric UI
		MetricsLabel.setFont(new Font("Tahoma", Font.BOLD, 13));
		MetricsLabel.setBounds(951, 37, 67, 14); // metrics
		contentPane.add(MetricsLabel);
		MostSoldLabel.setBounds(870, 87, 166, 14); // top selling
		contentPane.add(MostSoldLabel);
		
		MostProfitLabel.setBounds(870, 231, 166, 14); // most profitable
		contentPane.add(MostProfitLabel);
		
		RestockLabel.setBounds(870, 427, 166, 14); // needs restocked
		contentPane.add(RestockLabel);
		
		ProductListLabel.setFont(new Font("Tahoma", Font.BOLD, 13));
		ProductListLabel.setBounds(27, 214, 88, 14);
		
		contentPane.add(ProductListLabel);
		WeightLabel.setBounds(261, 65, 120, 14);
		
		contentPane.add(WeightLabel);
		QuantityLabel.setBounds(261, 87, 109, 14);
		
		contentPane.add(QuantityLabel);
		RestockNotificationLabel.setBounds(261, 112, 158, 14);
		
		contentPane.add(RestockNotificationLabel);
		Restock_textField.setColumns(10);
		Restock_textField.setBounds(429, 112, 86, 20);
		
		contentPane.add(Restock_textField);
		Quantity_textField.setColumns(10);
		Quantity_textField.setBounds(429, 88, 86, 20);
		
		contentPane.add(Quantity_textField);
		Weight_textField.setColumns(10);
		Weight_textField.setBounds(429, 65, 86, 20);
		
		contentPane.add(Weight_textField);
		product_pane.setBounds(24, 242, 795, 410);
		
		contentPane.add(product_pane);
		
		product_pane.setViewportView(product_table);
		mostSold_pane.setBounds(870, 112, 369, 108);
		
		contentPane.add(mostSold_pane);
		
		mostSold_pane.setViewportView(mostSold_table);
		margin_pane.setBounds(870, 256, 369, 160);
		
		contentPane.add(margin_pane);
		
		margin_pane.setViewportView(highMargin_table);
		restock_pane.setBounds(870, 452, 369, 200);
		
		contentPane.add(restock_pane);
		
		restock_pane.setViewportView(restock_table);
		
		RemoveProdLabel.setFont(new Font("Tahoma", Font.BOLD, 13));
		RemoveProdLabel.setBounds(653, 37, 166, 14);
		
		contentPane.add(RemoveProdLabel);
		rmvID_label.setBounds(653, 68, 120, 14);
		
		contentPane.add(rmvID_label);
		
		contentPane.add(rmv_textField);
		
		rmv_button.setBounds(653, 133, 89, 23);
		
		contentPane.add(rmv_button);
		outgoingContents_pane.setBounds(694, 113, 342, 362);
		
		contentPane.add(outgoingContents_pane);
		
		outgoingContents_pane.setViewportView(orderContents_table);
		lblNewLabel.setBounds(261, 137, 158, 14);
		contentPane.add(lblNewLabel);
		contentPane.add(totalSold_textField);
		update_button.setBounds(300, 196, 142, 23);
		contentPane.add(update_button);
		outgoingOrders_pane.setBounds(186, 114, 481, 361);
		contentPane.add(outgoingOrders_pane);
		outgoingOrders_pane.setViewportView(orders_table);
		fetchOrderContents_button.setBounds(459, 520, 197, 23);
		
		contentPane.add(fetchOrderContents_button);
		outgoingContents_lbl.setFont(new Font("Tahoma", Font.BOLD, 13));
		outgoingContents_lbl.setBounds(838, 64, 178, 14);
		
		contentPane.add(outgoingContents_lbl);
		contentPane.add(orderID_textField);
		fetchOrder_lbl.setBounds(219, 520, 109, 14);
		contentPane.add(fetchOrder_lbl);
		ship_btnGroup.add(shipped_btn);
		shipped_btn.setBounds(182, 48, 166, 23);
		contentPane.add(shipped_btn);
		ship_btnGroup.add(unshipped_btn);
		
		unshipped_btn.setBounds(372, 48, 166, 23);
		contentPane.add(unshipped_btn);
		// now that everything is set, make it all visible and refresh data!
		buildMetricView();
		hideOutgoingView(); // this makes sure we don't see any elements we don't want at startup
	}
	
	public void addMainButtons()
	{
		empCust_btnGroup.add(EmployeeButton);
		EmployeeButton.setSelected(true);
		EmployeeButton.setBounds(6, 7, 198, 23);
		contentPane.add(EmployeeButton);
		empCust_btnGroup.add(Incoming_button);
		
		Incoming_button.setBounds(206, 7, 213, 23);
		contentPane.add(Incoming_button);
		
		MetricButton.setBounds(870, 7, 109, 23);
		contentPane.add(MetricButton);	
		MetricButton.setSelected(true);
		Incoming_button.setVisible(true);
	}
	
	public void buildMetricView() {
		
		// turn on all elements we want to see in this view
		ID_textField.setVisible(true);
		Name_textField.setVisible(true);
		Category_textField.setVisible(true);
		Wholesale_textField.setVisible(true);
		Retail_textField.setVisible(true);
		Restock_textField.setVisible(true);
		Quantity_textField.setVisible(true);
		Weight_textField.setVisible(true);
		MetricsLabel.setVisible(true);
		ProductListLabel.setVisible(true);
		WeightLabel.setVisible(true);
		QuantityLabel.setVisible(true);
		RestockNotificationLabel.setVisible(true);
		MostSoldLabel.setVisible(true);
		MostProfitLabel.setVisible(true);
		RestockLabel.setVisible(true);
		AddProductLabel.setVisible(true);
		ID_Label.setVisible(true);
		NameLabel.setVisible(true);
		CategoryLabel.setVisible(true);
		WholesaleLabel.setVisible(true);
		RetailPriceLabel.setVisible(true);
		addProductButton.setVisible(true);
		
		totalSold_textField.setVisible(true);
		lblNewLabel.setVisible(true);
		update_button.setVisible(true);
		
		RemoveProdLabel.setVisible(true);
		rmvID_label.setVisible(true);
		rmv_textField.setVisible(true);
		rmv_button.setVisible(true);
		
		// build any tables we need
		
		try {
			allProducts = connection.getProducts(); // fetch new resultset in case of any updates
			while (allProducts.next())
			{
				// build table model
				String att1 = allProducts.getString("product_id");
				String att2 = allProducts.getString("name");
				String att3 = allProducts.getString("product_type");
				String att4 = allProducts.getString("weight");
				String att5 = allProducts.getString("wholesale_price");
				String att6 = allProducts.getString("retail_price");
				String att7 = allProducts.getString("qty_in_stock");
				String att8 = allProducts.getString("restock_notification");
				String att9 = allProducts.getString("total_sold");
				
				product_tableModel.addRow(new Object[] {att1,att2,att3,att4,att5,att6,att7,att8,att9});
				
			}
			// set table to table model
			product_table.setModel(product_tableModel);
			// data is now displayed!
			
			// now repeat
			needsRestocked_products = connection.needRestock(); // get new resultset
			while (needsRestocked_products.next())
			{
				// build table model
				String att1 = needsRestocked_products.getString("product_id");
				String att2 = needsRestocked_products.getString("name");
				String att3 = needsRestocked_products.getString("restock_notification");
				String att4 = needsRestocked_products.getString("qty_in_stock");
				
				needsRestocked_tableModel.addRow(new Object[] {att1,att2,att3,att4});
				
			}
			// set table
			restock_table.setModel(needsRestocked_tableModel);
			
			// fetch new resultset
			highestMargin_products = connection.getHighestMargins();
			while (highestMargin_products.next())
			{
				// build table
				String att1 = highestMargin_products.getString("product_id");
				String att2 = highestMargin_products.getString("name");
				String att3 = highestMargin_products.getString("margin");
				String att4 = highestMargin_products.getString("total_sold");
				
				margin_tableModel.addRow(new Object[] {att1, att2,att3,att4});
				
			}
			highMargin_table.setModel(margin_tableModel);
	
			mostSold_products = connection.getMostSold(); // result set
			while (mostSold_products.next())
			{
				String att1 = mostSold_products.getString("product_id");
				String att2 = mostSold_products.getString("name");
				String att3 = mostSold_products.getString("retail_price");
				String att4 = mostSold_products.getString("total_sold");
				// put result set in tableModel
				mostSold_tableModel.addRow(new Object[] {att1,att2,att3,att4});
			}
			// put tableModel in result set!
			mostSold_table.setModel(mostSold_tableModel);
		} catch (SQLException e1) {
			e1.printStackTrace(); // debugging
		}
		
		// turn on scroll panes
		// these scroll panes contain the tables we just added all our data to,
		// so now we need to set them visible to see our data
		mostSold_pane.setVisible(true);
		margin_pane.setVisible(true);
		restock_pane.setVisible(true);
		product_pane.setVisible(true);
		product_table.setVisible(true);
	}
	
	public void buildOutgoingView() {
		// set visible any elements we want to see
		outgoingContents_pane.setVisible(true);
		fetchOrderContents_button.setVisible(true);
		outgoingOrders_pane.setVisible(true);
		outgoingContents_lbl.setVisible(true);
		fetchOrder_lbl.setVisible(true);
		orderID_textField.setVisible(true);
		shipped_btn.setVisible(true);
		unshipped_btn.setVisible(true);
		
		try {
			// because we don't know what order they want
			// when they first get on this screen,
			// we do not show any data in the table that shows
			// the contents of the order, so we only build it when we need to.
			if (!initialization)
			{
				order_contents = connection.getOutgoingContents(orderID_textField.getText());
				while (order_contents.next())
				{
					String att1 = order_contents.getString("product_id");
					String att2 = order_contents.getString("quantity");
					
					contents_tableModel.addRow(new Object[] {att1,att2});
					
				}
				orderContents_table.setModel(contents_tableModel);
			}
			
			// get new resultset
			// we use the same table here
			// we just need to know what the user wants to see, so we just need
			// to check and then change the resultset accordingly.
			if (shipped_btn.isSelected())
			{
				orders_RS = connection.getShippedOrders();
			}
			else
			{
				orders_RS = connection.getUnshippedOrders();
			}
			
			while (orders_RS.next())
			{
				String att1 = orders_RS.getString("order_id");
				String att2 = orders_RS.getString("date_placed");
				String att3 = orders_RS.getString("shipping_id");
				orders_tableModel.addRow(new Object[] {att1,att2,att3});
				
			}
			orders_table.setModel(orders_tableModel);
			
		} catch (SQLException e1) {
			e1.printStackTrace(); // debugging
		}
	}
	
	public void buildIncomingView() {
		outgoingContents_pane.setVisible(true);
		fetchOrderContents_button.setVisible(true);
		outgoingOrders_pane.setVisible(true);
		outgoingContents_lbl.setVisible(true);
		fetchOrder_lbl.setVisible(true);
		orderID_textField.setVisible(true);
		shipped_btn.setVisible(false);
		unshipped_btn.setVisible(false);
		// then make sure hideShipping is good (setvisible)
		try {
			if (!initialization)
			{
				order_contents = connection.getIncomingContents(orderID_textField.getText());
				while (order_contents.next())
				{
					String att1 = order_contents.getString("product_id");
					String att2 = order_contents.getString("quantity");
					
					contents_tableModel.addRow(new Object[] {att1,att2});
					
				}
				orderContents_table.setModel(contents_tableModel);
			}
			
			orders_RS = connection.getIncomingOrders();
			while (orders_RS.next())
			{
				String att1 = orders_RS.getString("supplier");
				String att2 = orders_RS.getString("total_price");
				String att3 = orders_RS.getString("tracking_id");
				orders_tableModel.addRow(new Object[] {att1,att2,att3});
				
			}
			orders_table.setModel(orders_tableModel);
			
		} catch (SQLException e1) {
			e1.printStackTrace(); // debugging
		}
	}
	
	public void hideMetricView() {
		// reverse of what we were doing before
		// make everything we don't want to see invisible
		ID_textField.setVisible(false);
		Name_textField.setVisible(false);
		Category_textField.setVisible(false);
		Wholesale_textField.setVisible(false);
		Retail_textField.setVisible(false);
		Restock_textField.setVisible(false);
		Quantity_textField.setVisible(false);
		Weight_textField.setVisible(false);
		totalSold_textField.setVisible(false);
		lblNewLabel.setVisible(false);
		update_button.setVisible(false);
		MetricsLabel.setVisible(false);
		ProductListLabel.setVisible(false);
		WeightLabel.setVisible(false);
		QuantityLabel.setVisible(false);
		RestockNotificationLabel.setVisible(false);
		MostSoldLabel.setVisible(false);
		RestockLabel.setVisible(false);
		MostProfitLabel.setVisible(false);
		AddProductLabel.setVisible(false);
		ID_Label.setVisible(false);
		NameLabel.setVisible(false);
		CategoryLabel.setVisible(false);
		WholesaleLabel.setVisible(false);
		RetailPriceLabel.setVisible(false);
		addProductButton.setVisible(false);
		RemoveProdLabel.setVisible(false);
		rmvID_label.setVisible(false);
		rmv_textField.setVisible(false);
		rmv_button.setVisible(false);
		
			// only remove elements we need to!
		while (product_tableModel.getRowCount() > 0)
			product_tableModel.removeRow(0);
		while (needsRestocked_tableModel.getRowCount() > 0)
			needsRestocked_tableModel.removeRow(0);
		while (margin_tableModel.getRowCount() > 0)
			margin_tableModel.removeRow(0);
		while (mostSold_tableModel.getRowCount() > 0)
			mostSold_tableModel.removeRow(0);
		
		// now set the panes to invisible
		product_table.setVisible(false);
		mostSold_pane.setVisible(false);
		margin_pane.setVisible(false);
		restock_pane.setVisible(false);
		product_pane.setVisible(false);
	}
	
	public void hideOutgoingView() {
		// only remove elements we need to!
		
		// not many elements here so let's just set everything invisible
		outgoingContents_pane.setVisible(false);
		fetchOrderContents_button.setVisible(false);
		outgoingOrders_pane.setVisible(false);
		outgoingContents_lbl.setVisible(false);
		fetchOrder_lbl.setVisible(false);
		orderID_textField.setVisible(false);
		shipped_btn.setVisible(false);
		unshipped_btn.setVisible(false);
		
		while (orders_tableModel.getRowCount() > 0)
			orders_tableModel.removeRow(0);
		while (contents_tableModel.getRowCount() > 0)
			contents_tableModel.removeRow(0);
	}
	
	public void hideIncomingView() {
		outgoingContents_pane.setVisible(false);
		fetchOrderContents_button.setVisible(false);
		outgoingOrders_pane.setVisible(false);
		outgoingContents_lbl.setVisible(false);
		fetchOrder_lbl.setVisible(false);
		orderID_textField.setVisible(false);
		shipped_btn.setVisible(false);
		unshipped_btn.setVisible(false);
		
		while (orders_tableModel.getRowCount() > 0)
			orders_tableModel.removeRow(0);
		while (contents_tableModel.getRowCount() > 0)
			contents_tableModel.removeRow(0);
	}
		
	public void runProgram()
	{
		
		// this function contains all the handlers
		// this is essentially the functionality of the GUI
		// when certain buttons are clicked, views are either built or hidden
		// some buttons require queries to function (the add button)
		EmployeeButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				hideMetricView(); // this was chosen to be the default view. this prevents a UI bug where the data is duplicated in the tables. 
				hideOutgoingView(); // this ensures a similar thing.
				initialization = true;
				boolean EmployeeView = EmployeeButton.isSelected();
				if (EmployeeView)
				{
					// set up view
					MetricButton.setVisible(true);
					MetricButton.setSelected(true);
					hideIncomingView();
					buildMetricView();
				}
				else
				{
					// load customer view
					MetricButton.setVisible(false);
					hideMetricView();
					buildIncomingView();
				}
			}
		});
		Incoming_button.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				boolean Incoming = Incoming_button.isSelected();
				if (Incoming)
				{
					// build view
					hideOutgoingView();
					hideMetricView();
					buildIncomingView();
					MetricButton.setVisible(false);
				}
				else
				{
					hideOutgoingView();
					hideIncomingView();
					hideMetricView();
					buildMetricView();
					MetricButton.setSelected(true);
				}
			}
		});
			MetricButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					hideMetricView(); // this was chosen to be the default view. this prevents a UI bug where the data is duplicated in the tables.
					boolean MetricsView = MetricButton.isSelected();
					shipped_btn.setSelected(true);
					initialization = true;
					if (MetricsView)
					{
						buildMetricView();
						hideOutgoingView();
					}
					else
					{ 
						hideMetricView();
						buildOutgoingView();
					}
				}
			});
		
			// add new product
		addProductButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// grab text field vals, execute query
				// all inputs are received as strings and since using strings makes
				// queries easy, no need to type-cast
				// however, types in DB are commented
				String product_id, qty_in_stock, restock_notification, total_sold; // ints
				String name, category; // strings
				String weight, wholesale_price, retail_price; // floats
				String msg = "";
				try {
					// get inputs
					product_id = ID_textField.getText();
					qty_in_stock = Quantity_textField.getText();
					restock_notification = Restock_textField.getText();
					total_sold = "0";
					weight = Weight_textField.getText();
					wholesale_price = Wholesale_textField.getText();
					retail_price = Retail_textField.getText();
					name = Name_textField.getText();
					category = Category_textField.getText();
					
					// try to add product, get error msg if needed.
					connection.addProduct(product_id,name, category, weight, wholesale_price, retail_price, qty_in_stock, restock_notification, total_sold);
					System.out.println(msg);
					
					// the catch block no longer catches exceptions because they are of a different type.
					if (msg.contains("Duplicate"))
					{
						// Popup
						JOptionPane.showMessageDialog(null, msg, "Error", JOptionPane.INFORMATION_MESSAGE);
					}
					else if (msg.contains("error"))
					{
						// Popup
						JOptionPane.showMessageDialog(null, "Please check all fields.", "Error", JOptionPane.INFORMATION_MESSAGE);
					}
					
					// refresh so table updates w new item
					hideMetricView();
					buildMetricView();
				}
				catch (SQLException exc)
				{
					JOptionPane.showMessageDialog(null, exc.getMessage() + " " + msg, "Error", JOptionPane.INFORMATION_MESSAGE);
					System.out.println(exc.getMessage());
				}
				
			}
		});
		
		rmv_button.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				int ID = Integer.parseInt(rmv_textField.getText());
				connection.deleteProduct(ID);
				
				// refresh current page
				hideMetricView();
				buildMetricView();
			}
		});
		update_button.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String product_id, qty_in_stock, restock_notification, total_sold; // ints
				String name, category; // strings
				String weight, wholesale_price, retail_price; // floats
				String msg = "";
				try {
					// get inputs
					product_id = ID_textField.getText();
					qty_in_stock = Quantity_textField.getText();
					restock_notification = Restock_textField.getText();
					total_sold = totalSold_textField.getText();
					weight = Weight_textField.getText();
					wholesale_price = Wholesale_textField.getText();
					retail_price = Retail_textField.getText();
					name = Name_textField.getText();
					category = Category_textField.getText();
					
					// try to add product, get error msg if needed.
					connection.updateProduct(product_id, name, category, weight, wholesale_price, retail_price, qty_in_stock, restock_notification, total_sold);
					System.out.println(msg);
					
					// the catch block no longer catches exceptions because they are of a different type.
					if (msg.contains("Duplicate"))
					{
						// Popup
						JOptionPane.showMessageDialog(null, msg, "Error", JOptionPane.INFORMATION_MESSAGE);
					}
					else if (msg.contains("error"))
					{
						// Popup
						JOptionPane.showMessageDialog(null, "Please check all fields.", "Error", JOptionPane.INFORMATION_MESSAGE);
					}
					
					// refresh so table updates w new item
					hideMetricView();
					buildMetricView();
				}
				catch (SQLException exc)
				{
					JOptionPane.showMessageDialog(null, exc.getMessage() + " " + msg, "Error", JOptionPane.INFORMATION_MESSAGE);
					System.out.println(exc.getMessage());
				}
			}
		});
		
		// Shipping view handlers
		fetchOrderContents_button.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String t = orderID_textField.getText();
				if (t == null || t.isBlank() || t.isEmpty() || t == "")
				{
					JOptionPane.showMessageDialog(null, "Text field is empty. Please enter an order ID.", "Error", JOptionPane.INFORMATION_MESSAGE);
					initialization = true; 
				}
				
				// refresh
				if (Incoming_button.isSelected())
				{
					hideIncomingView();
					buildIncomingView();
				}
				else
				{
					hideOutgoingView();
					buildOutgoingView();
				}
				outgoingContents_lbl.setText("Contents for order #  " + orderID_textField.getText()); 
				initialization = false;
			}
		});
		shipped_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				initialization = true;
				if (Incoming_button.isSelected())
				{
					hideIncomingView();
					buildIncomingView();
				}
				else
				{
					hideOutgoingView();
					buildOutgoingView();
				}
				
			}
		});
		unshipped_btn.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				initialization = true;
				if (Incoming_button.isSelected())
				{
					hideIncomingView();
					buildIncomingView();
				}
				else
				{
					hideOutgoingView();
					buildOutgoingView();
				}
				
			}
		});
		project_GUI.this.revalidate();
		project_GUI.this.repaint();
	}
}
