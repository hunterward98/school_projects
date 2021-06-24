/* AUTHORS: Hunter Ward and Matthew Schwartz
 * FILE: Queries.java
 * DESC: This file implements a class that we can use to connect to a database.
 * This class returns mostly resultsets, such that we can easily see the data
 * in a table. All of these queries/functions are self-explanatory.
 * addProduct() just adds a product to the product table, etc.
 */

package newproject;

import java.sql.*;
import java.util.Scanner;

public class Queries {
	private Connection cn;
	private Statement st;
	public Queries(){
		Scanner scan = new Scanner(System.in);
		// this URL took quite a bit of time to figure out after googling several error msgs
		String url = "jdbc:mysql://cps-database.gonzaga.edu/mschwartz_DB?database=mschwartz_DB&serverTimezone=PST";
		String user = "USER";
		String pswd = "%PASS";
		try {
			cn = DriverManager.getConnection(url,user,pswd);
			st = cn.createStatement();
		}
		catch(Exception e) {
			System.out.println(e);
		}
	}
	
	public String addProduct(String product_id, String name, String category, String weight, String wholesale_price, String retail_price, String qty_in_stock, String restock_notification, String total_sold) throws SQLException {
		
		total_sold = "0"; // should always be zero when adding a new product, none have been sold yet
		String result = "";
		String[] inputs = {product_id, name, category, weight, wholesale_price, retail_price, qty_in_stock, restock_notification};
		int i = 0;
		int pos = 0;
		String details = "";
		for(i = 0; i < 8; i++)
		{
			// check input to give better error feedback
			// this way the user doesn't see SQL related msgs
			// so they don't have to understand SQL to fix it
			if(!inputIsGood(inputs[i], i))
			{
				if (i == 0)
				{
					// this means the ID field was checked and the input is not good.
					details = "ID exists or is not a positive int.";
					pos = i;
				}
				else if (i == 2 || i == 1)
				{
					// most common way to get this error:
					// empty string in a text field
					// malicious input (semicolons, etc)
					// product name is an extreme situation and contains the exact phrase "OR"
					details = "Check for unwanted characters in the Name or Category field and match convention.";
					pos = i;
				}
				else
				{
					details = "Please check number types. You may be accidentally entering floats/strings instead of integers.";
					pos = i;
				}
				throw new SQLException("Error with input: \"" + inputs[i] + "\". Info: " + details);
			}
		}
		
		String stmt ="INSERT INTO product VALUES(" + product_id + ", '" + name + "','" + category + "', " + weight + "," + wholesale_price + "," + retail_price + "," + qty_in_stock + "," + restock_notification + "," + total_sold + ");";
		System.out.println(stmt);
		try {
			st.executeUpdate(stmt);
		}
		catch (Exception e) {
			//System.out.println(e); // for debugging
			result = "Error with input: \"" + inputs[i] + "\". Info: " + details;
		}
		return result;
	}
	
	private boolean inputIsGood(String input, int num)
	{
		// validate inputs
		// helps give good user feedback and also prevents accidental injection
		if (input.isEmpty())
		{
			return false;
		}
		if (num != 1 && num != 2)
		{
			if (Float.parseFloat(input) < 0)
			{
				return false;
			}
		}
		if (input.contains(";") || input.contains("--") || input.contains("OR")) 
		{
			return false;
		}
		return true;
	}
	
	public ResultSet getOutgoingContents(String order_id){
        String stmt = "SELECT product_id, quantity FROM outgoing_contents WHERE order_id = "+order_id+";";
        ResultSet rs = null;
        try{
            rs = st.executeQuery(stmt);
            return rs;
        }
        catch(Exception e){
            System.out.println(e);
        }
        return rs;
    }
    
    public void updateProduct(String product_id, String name, String category, String weight, String wholesale_price, String retail_price, String qty_in_stock, String restock_notification, String total_sold) throws SQLException{
        String[] inputs = {product_id, name, category, weight, wholesale_price, retail_price, qty_in_stock, restock_notification, total_sold};
        for(int i = 0; i < 9; i++)
        {
            if (inputIsGood(inputs[i], i))
            {
                continue;
            }
            else
            {
                String err = "Failed on input: " + inputs[i] + ". Please check value types.";
                throw new SQLException(err);
            }
        }
        String stmt ="UPDATE product SET product_id = "+inputs[0]+
        ", name = '"+inputs[1]+"', product_type = '"+inputs[2]+"', weight = "+inputs[3]+
        ", wholesale_price = "+inputs[4]+", retail_price = "+inputs[5]+", qty_in_stock = "+
        inputs[6]+", restock_notification = "+inputs[7]+", total_sold = "+inputs[8]+" WHERE product_id = "+product_id+";";
        try {
            st.executeUpdate(stmt);
        }
        catch (Exception e) {
            System.out.println(e); // for debugging
        }
    }
	
	public ResultSet getProducts(){
		String stmt =" SELECT product_id, name, product_type, weight, wholesale_price, retail_price, qty_in_stock, restock_notification, total_sold FROM product;"; 
		ResultSet rs = null;
		try{
			
			rs = st.executeQuery(stmt);
			return rs;
		}
		catch(Exception e){
			System.out.println(e);
		}
		return rs;
	}
	
	public ResultSet getHighestMargins(){
		String stmt = "SELECT product_id, name, retail_price - wholesale_price as margin, total_sold FROM product ORDER BY margin DESC LIMIT 5;"; 
		ResultSet rs = null;
		try{
			
			rs = st.executeQuery(stmt);
			return rs;
		}
		catch(Exception e){
			System.out.println(e);
		}
		return rs;
	}
	
	public ResultSet getMostSold(){
		String stmt = "SELECT product_id, name, retail_price, total_sold FROM product WHERE total_sold > 0 ORDER BY total_sold DESC LIMIT 5;"; 
		ResultSet rs = null;
		try{
			
			rs = st.executeQuery(stmt);
			return rs;
		}
		catch(Exception e){
			System.out.println(e);
		}
		return rs;
	}
	
	public ResultSet needRestock(){
		String stmt = "SELECT product_id, name, restock_notification, qty_in_stock FROM product WHERE restock_notification >= qty_in_stock;";
		ResultSet rs = null;
		try{
			rs = st.executeQuery(stmt);
			return rs;
		}
		catch(Exception e){
			System.out.println(e);
		}
		return rs;
	}
	
	public void deleteProduct(int id) {
        String stmt = "DELETE FROM product WHERE product_id = "+ id +";";
        try {
            st.executeUpdate(stmt);
        }
        catch (Exception e) {
            System.out.println(e); // for debugging
        }
    }
	
	public ResultSet getUnshippedOrders(){
		String stmt = "SELECT order_id, date_placed, shipping_id FROM outgoing_orders WHERE shipping_id IS NULL;";
		ResultSet rs = null;
		try{
			rs = st.executeQuery(stmt);
			return rs;
		}
		catch(Exception e){
			System.out.println(e);
		}
		return rs;
	}
	public ResultSet getShippedOrders(){
		String stmt = "SELECT order_id, date_placed, shipping_id FROM outgoing_orders WHERE shipping_id IS NOT NULL;";
		ResultSet rs = null;
		try{
			rs = st.executeQuery(stmt);
			return rs;
		}
		catch(Exception e){
			System.out.println(e);
		}
		return rs;
	}
	public ResultSet getIncomingOrders(){
		String stmt = "SELECT supplier, total_price, tracking_id FROM incoming_order;";
		ResultSet rs = null;
		try{
			rs = st.executeQuery(stmt);
			return rs;
		}
		catch(Exception e){
			System.out.println(e);
		}
		return rs;
	}
	public ResultSet getIncomingContents(String orderID){
		String stmt = "SELECT product_id, quantity FROM incoming_contents WHERE tracking_id ="+orderID+";";
		ResultSet rs = null;
		try{
			rs = st.executeQuery(stmt);
			return rs;
		}
		catch(Exception e){
			System.out.println(e);
		}
		return rs;
	}
}