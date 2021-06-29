This folder contains a final project that was worked on for the majority
of the semester. Everything we learned in this class was used in this
project, so I think it's perfect to include it.

These two java files together create a program that's intended to
be used for inventory management. There is also some basic
stats shown (I believe these were things that needed to be
restocked, most sold, and highest profit), but also a view that
allows someone to check outgoing orders for packing.
Unfortunately, this project no longer works mostly because there
is no database for it to connect to. Because it relies on the
result sets actually returning something other than null, even if
the resultset is empty, it's simply not worth having a SQL server
just for this project. If there was a way to hardcode an empty resultset,
it's likely the code could be modified such that the GUI would
work completely. However, most of the GUI relies on tables.
But just "for fun," I have included a jar file which is the very
first time we were able to get the application to run properly.
For me this is nostaligc, I remember how good it felt to have
something like this running when I had never created a GUI before.
This is when the MainAppFrame.java file likely had about 150 lines
of code, compared to the 860 lines it currently has. If you cannot
get it to work, you may have to install the Java SDK.

Anyways, this was a project that was worked on for most of the semester
for a database management class. Over the course of about ten weeks,
we created what would eventually become this application. It began
with creating a proposal, a basic schema, then as we learned about
relationships we had to adjust our schema. We also created an ER
diagram and a SQL file to implement our schema on the server provided
by the University, but also did some other things such as do small example
queries to show it works and do a rough sketch of our UI.

We were given a choice on how we wanted to do our UI, so we opted for a
GUI that uses Java. The largest reason for this was I had no experience with
Java or creating a GUI, and it was definitely going to keep it
interesting compared to a console UI. This certainly proved to be
challenging, and I'm sure that there are some things that I did that
may be unconventional or just straight up weird to someone else.
This also allowed for me to work with Eclispe IDE for the first time.

implement.sql is a file we had submitted that built upon a previous
project update. It creates all the tables for our data base, has
example queries, and populates some tables.