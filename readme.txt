Purpose: A program that reads in a file that contains the following information about adults:
<age, workclass, final_weight, education, education_number, marital_status, occupation, relationship, 
race, sex, capital_gain, capital_loss, hours_worked_per_week, native_country, salary>

It then stratify samples the information, in an effort to perform a Naive Bayesian algorithm on the
data, to see how close each adult's information is to each other. It compares how accurate the Naive Bayesian
algorithm differs depending on how large of a training set is set. 

How to compile: g++ -o naiveBayesian assignment2.cpp
How to run: ./naiveBayesian adult.data.txt
