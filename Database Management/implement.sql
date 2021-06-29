-- Hunter Ward and Matthew Schwartz
-- Project 5
-- file name: proj5.sql
-- The first part is stuff from project update 4. Please scroll until you see project 5.

CREATE TABLE product(
	product_id INT UNSIGNED,
	name VARCHAR(50),
	product_type VARCHAR(50),
	weight DOUBLE UNSIGNED,
	wholesale_price DECIMAL(18,2),
	retail_price DECIMAL(18,2),
	qty_in_stock INT UNSIGNED,
	restock_notification INT UNSIGNED,
	total_sold INT UNSIGNED,
	PRIMARY KEY (product_id)
);

CREATE TABLE incoming_order(
	tracking_id INT UNSIGNED,
	total_price DECIMAL(18,2),
	date_received DATE,
	supplier VARCHAR(50),
	PRIMARY KEY(tracking_id)
);

CREATE TABLE incoming_contents(
	tracking_id INT UNSIGNED,
	product_id INT UNSIGNED,
	quantity INT UNSIGNED,
	PRIMARY KEY (tracking_id, product_id),
	FOREIGN KEY (tracking_id) REFERENCES incoming_order(tracking_id),
	FOREIGN KEY (product_id) REFERENCES product(product_id)
);

CREATE TABLE outgoing_orders(
	order_id INT UNSIGNED,
	total_price DECIMAL(18,2),
	shipping_id INT UNSIGNED,
	date_placed DATE,
	total_weight DOUBLE,
	PRIMARY KEY (order_id)
);

CREATE TABLE outgoing_contents(
	order_id INT UNSIGNED,
	product_id INT UNSIGNED,
	quantity INT UNSIGNED,
	PRIMARY KEY (order_id, product_id),
	FOREIGN KEY (product_id) REFERENCES product(product_id),
	FOREIGN KEY (order_id) REFERENCES outgoing_orders(order_id)
);

INSERT INTO product VALUES (69,"cream cheese", "dairy", 2, 2.00, 3.99, 12, 0, 314);
INSERT INTO outgoing_orders VALUES (12, 300.59, 420, '2020-10-22', 50);
INSERT INTO outgoing_contents VALUES (12, 69, 100);
INSERT INTO product VALUES(9493, 'Waffle Mix', 'Baking', 0.855, 2.19, 4.95, 2, 10, 0);
INSERT INTO product VALUES(9492, 'Pancake Mix', 'Baking', 0.855, 2.19, 4.95, 200, 10, 0);
INSERT INTO product VALUES(9494, 'Unbleached Flour', 'Baking', 1.0, 1.08, 3.95, 500, 50, 0);
INSERT INTO product VALUES(9495, 'Powdered Sugar', 'Baking', 0.50, 0.85, 4.95, 100, 10, 0);
INSERT INTO product VALUES(9496, 'Baking Soda', 'Baking', 1.250, 1.83, 5.95, 500, 25, 0);
INSERT INTO product VALUES(9497, 'Baking Powder', 'Baking', 1.250, 1.89, 5.95, 500, 25, 0);
INSERT INTO product VALUES(9498, 'Corn Starch', 'Baking', 0.50, 0.98, 2.95, 100, 5, 0);
INSERT INTO incoming_order VALUES(858283494, 1347, '2020-06-09', 'Super good baking goods');
INSERT INTO incoming_contents VALUES(858283494, 9493, 200); 
INSERT INTO incoming_contents VALUES(858283494, 9492, 200); 
INSERT INTO incoming_contents VALUES(858283494, 9494, 500); 
INSERT INTO incoming_contents VALUES(858283494, 9495, 100); 
INSERT INTO incoming_contents VALUES(858283494, 9496, 500); 
INSERT INTO incoming_contents VALUES(858283494, 9497, 500);
INSERT INTO incoming_contents VALUES(858283494, 9498, 100); 
INSERT INTO incoming_order VALUES(858283495, 1347, NULL, 'Super duper good baking goods');
INSERT INTO incoming_contents VALUES(858283495, 9493, 200); 

SELECT * FROM incoming_contents;
SELECT * FROM incoming_order;
SELECT * FROM outgoing_contents;
SELECT * FROM outgoing_orders;
SELECT * FROM product;

-- ----------------------------------------------------------------------------
-- START PROJECT 5 WORK
-- ----------------------------------------------------------------------------

-- Look up price of a specific item for the price of an order
SET @item = 69;
SELECT product_id, retail_price
FROM product
WHERE product_id = @item;

-- Look up the weight of a specific item for shipping information of an order
SET @item = 69;
SELECT product_id, weight
FROM product
WHERE product_id = @item;

-- Show the contents of an outgoing order
SET @order_num = 12;
SELECT order_id, product_id, quantity
FROM outgoing_contents
WHERE order_id = @order_num;

-- Show the contents of an incoming order
SET @tracking_num = 858283494;
SELECT i1.tracking_id, i2.product_id, i2.quantity
FROM incoming_order i1, incoming_contents i2
WHERE i1.tracking_id = @tracking_num;

-- Number of itemms sold (will be used to calc total profits)
SET @item = 69;
SELECT total_sold, wholesale_price, retail_price
FROM product
WHERE product_id = @item;

-- Find all items that need to be restocked
SELECT product_id, name, restock_notification, qty_in_stock
FROM product
WHERE restock_notification >= qty_in_stock;

-- Find all orders that have not been received
SELECT tracking_id
FROM incoming_order
WHERE date_received IS NULL;

-- Return most expensive outgoing/incoming order
SELECT MAX(o.total_price) AS most_expensive_outgoing, MAX(i.total_price) AS most_expensive_incoming
FROM outgoing_orders o, incoming_order i;
-- ????

-- Order information that will be sent to customer
SET @id = 12;
SELECT o1.order_id, o1.total_price, o1.date_placed, o1.shipping_id, o2.product_id, o2.quantity
FROM outgoing_contents o2, outgoing_orders o1
WHERE o1.order_id = @id;

-- ----------------------------------------------------------------------------
-- END PROJECT 5 WORK
-- ----------------------------------------------------------------------------