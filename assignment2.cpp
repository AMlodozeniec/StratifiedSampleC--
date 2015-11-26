#include <iostream> //std::cout
#include <fstream> //std::ifstream
#include <string> //std::string
#include <sstream> //std::istringstream
#include <vector> //std::vector
#include <algorithm> //std::remove
#include <ctime> //std::time

#define NUM_OF_ITERATIONS 10

struct Person{
	int age;
	std::string workclass;
	int fnlwgt;
	std::string education;
	int educationNum;
	std::string maritalStatus;
	std::string occupation;
	std::string relationship;
	std::string race;
	std::string sex;
	int capitalGain;
	int capitalLoss;
	int hoursPerWeek;
	std::string nativeCountry;
	std::string salary;
};

struct Count {
	double pos = 0;
	double neg = 0;
	void increment(bool flag){
		if(flag){
			++pos;
		}
		else{
			++neg;	
		}
	}
};

//Increments count of data attribute. If it returns true for salary is over 50k, it increments the positive. Otherwise, it incrementss the negative
template <typename T>
Count compute_count(const Person &sample, const std::vector<Person> &trainingSet, T Person::*member){
	Count res;
	for(int i = 0; i < trainingSet.size(); i++){
		Person training = trainingSet[i];
		if((sample.*member) == (training.*member)){
			res.increment(training.salary == ">50K");
		}
	}
	return res;
}

//Generates random number between 0 and the max
int randNumGenerator(int max){
	int num = (rand() % max);
	return num;
}

//Sets number of people with salaries over 50k and under 50k
void setNumCounts(int &over50k, int &under50k, std::vector<Person> trainingSet){
	for(int i = 0; i < trainingSet.size(); i++){
		if(trainingSet[i].salary == ">50K"){
			over50k++;
		}
		else{
			under50k++;
		}
	}
}

//Implements naiveBayesian algorithm on training set and testing set
float naiveBayesian(std::vector<Person> trainingSet, std::vector<Person> testingSet){
	double accuracy = 0;
	int accuracyCount, randNum =  0;
	
	std::vector<Person> sampleSet;
	std::vector<bool> numsUsedAlready(testingSet.size());
	
	for(int i = 0; i < 20; i++){
		randNum = randNumGenerator(testingSet.size());
		while(numsUsedAlready[randNum]){
			randNum = randNumGenerator(testingSet.size());
		}
		numsUsedAlready[randNum] = true;
		sampleSet.push_back(testingSet[randNum]);
	}

	int numOver50k = 0;
	int numUnder50k = 0;
	setNumCounts(numOver50k, numUnder50k, trainingSet);
	
	double probOver50k = (double)numOver50k / trainingSet.size();
	double probUnder50k = (double)numUnder50k / trainingSet.size();
	
	for(int i = 0; i < sampleSet.size(); i++){
		Person sample = sampleSet[i];
		
		Count age, workclass, fnlwgt, education,
			educationNum, maritalStatus, occupation, race,
			relationship, sex, capitalGain, capitalLoss, 
			hoursPerWeek, nativeCountry;
		
		age = compute_count(sample, trainingSet, &Person::age);
		workclass = compute_count(sample, trainingSet, &Person::workclass);
		fnlwgt = compute_count(sample, trainingSet, &Person::fnlwgt);
		education = compute_count(sample, trainingSet, &Person::education);
		educationNum = compute_count(sample, trainingSet, &Person::educationNum);
		maritalStatus = compute_count(sample, trainingSet, &Person::maritalStatus);
		occupation = compute_count(sample, trainingSet, &Person::occupation);
		race = compute_count(sample, trainingSet, &Person::race);
		relationship = compute_count(sample, trainingSet, &Person::relationship);
		sex = compute_count(sample, trainingSet, &Person::sex);
		capitalGain = compute_count(sample, trainingSet, &Person::capitalGain);
		capitalLoss = compute_count(sample, trainingSet, &Person::capitalLoss);
		hoursPerWeek = compute_count(sample, trainingSet, &Person::hoursPerWeek);
		nativeCountry = compute_count(sample, trainingSet, &Person::nativeCountry);
		
		double probYes = 1.0 * (age.pos / numOver50k)
				     * (workclass.pos / numOver50k)
				     * (fnlwgt.pos / numOver50k)
				     * (education.pos / numOver50k)
				     * (educationNum.pos / numOver50k)
				     * (maritalStatus.pos / numOver50k)
				     * (occupation.pos / numOver50k)
				     * (race.pos / numOver50k)
				     * (relationship.pos / numOver50k)
				     * (sex.pos / numOver50k)
				     * (capitalGain.pos / numOver50k)
				     * (capitalLoss.pos / numOver50k)
				     * (hoursPerWeek.pos / numOver50k)
				     * (nativeCountry.pos / numOver50k);
		
		double probNo = 1.0 * (age.neg / numUnder50k)
				     * (workclass.neg / numUnder50k)
				     * (fnlwgt.neg / numUnder50k)
				     * (education.neg / numUnder50k)
				     * (educationNum.neg / numUnder50k)
				     * (maritalStatus.neg / numUnder50k)
				     * (occupation.neg / numUnder50k)
				     * (race.neg / numUnder50k)
				     * (relationship.neg / numUnder50k)
				     * (sex.neg / numUnder50k)
				     * (capitalGain.neg / numUnder50k)
				     * (capitalLoss.neg / numUnder50k)
				     * (hoursPerWeek.neg / numUnder50k)
				     * (nativeCountry.neg / numUnder50k);

			
		double yes = probYes * probOver50k;
		double no = probNo * probUnder50k;
		
		if(yes > no){
			if(sample.salary == ">50K"){
				accuracyCount++;
			}
		}
		else{
			if(sample.salary == "<=50K"){
				accuracyCount++;
			}
		}
	}//end outer loop
	
	accuracy = (double)accuracyCount / 20;
	return accuracy;
}

//Adds up all accuracies from each iteration to compute total accuracy for the % of testing set
double findAccuracy(std::vector<Person> stratifiedSet, std::vector<Person> testingSet){
	double accuracy, totalAccuracy;
	for(int i = 0; i < NUM_OF_ITERATIONS; i++){	
		accuracy = naiveBayesian(stratifiedSet, testingSet);
		totalAccuracy += accuracy;
	}
	totalAccuracy = totalAccuracy / NUM_OF_ITERATIONS;
	
	return totalAccuracy;
}

//Prints accuracy to output
void printAccuracy(double accuracy10, double accuracy30, double accuracy50){
	std::cout << "Accuracy for 10: " << accuracy10 << std::endl;
	std::cout << "Accuracy for 30: " << accuracy30 << std::endl;
	std::cout << "Accuracy for 50: " << accuracy50 << std::endl;
}

//Samples the dataset into a stratified set and puts the remaining data into a testing set
std::vector<Person> sample(std::vector<Person> wholeDataSet, int percentage, std::vector<Person> &testingSet){
	int wholeDataSize = wholeDataSet.size();
	
	std::vector<Person> stratifiedSet;
	
	int limit = (wholeDataSize * percentage) / 100;
	int randNum= 0;
	
	std::vector<bool> numsUsedAlready(wholeDataSize);

	for(int i = 0; i < limit; i++){
		randNum = randNumGenerator(wholeDataSize);
		while(numsUsedAlready[randNum]){
			randNum = randNumGenerator(wholeDataSize);
		}
		numsUsedAlready[randNum] = true;
		stratifiedSet.push_back(wholeDataSet[randNum]);	
	}
	
	//Push all nontaken values into testingSet
	for(int i = 0; i < numsUsedAlready.size(); i++){
		if(!numsUsedAlready[i]){
			testingSet.push_back(wholeDataSet[i]);
		}
	}
	
	return stratifiedSet;
}

//Concats the two vectors together to make one vector
std::vector<Person> concatVectors(std::vector<Person> a, std::vector<Person> b){
	std::vector<Person> ab;
	ab.reserve(a.size() + b.size());
	ab.insert(ab.end(), a.begin(), a.end()); //Add a
	ab.insert(ab.end(), b.begin(), b.end()); //Add b
	return ab;
}

//Pushes all data with salaries over 50k into positive sample, and same with negative (under 50k)
void setPosAndNegSamples(std::vector<Person> &positiveSamples, std::vector<Person> &negativeSamples, std::vector<Person> testData){
	for(int i = 0; i < testData.size(); i++){
		if(testData[i].salary == ">50K"){
			positiveSamples.push_back(testData[i]);
		}
		else{
			negativeSamples.push_back(testData[i]);
		}
	}
}

//Removes 10%, 30%, or 50% of the data into a trainingset and places the rest into a testing set.
void stratifiedSample(std::vector<Person> testData){
	srand(time(NULL)); 
	
	std::vector<Person> positiveSamples, negativeSamples;
	
	setPosAndNegSamples(positiveSamples, negativeSamples, testData);
	
	std::vector<Person> posTestingSet10 = positiveSamples;
	std::vector<Person> posTestingSet30 = positiveSamples;
	std::vector<Person> posTestingSet50 = positiveSamples;
	
	std::vector<Person> negTestingSet10 = negativeSamples;
	std::vector<Person> negTestingSet30 = negativeSamples;
	std::vector<Person> negTestingSet50 = negativeSamples;
	
	std::vector<Person> posStratifiedSet_10,  posTesting_10,
			    posStratifiedSet_30, posTesting_30,
			    posStratifiedSet_50, posTesting_50;

	std::vector<Person> negStratifiedSet_10, negTesting_10,
			    negStratifiedSet_30, negTesting_30,
			    negStratifiedSet_50, negTesting_50;
	
	std::vector<Person> stratifiedSet_10, testingSet_10,
			    stratifiedSet_30, testingSet_30,
			    stratifiedSet_50, testingSet_50;
	
	posStratifiedSet_10 = sample(posTestingSet10, 10, posTesting_10);
	negStratifiedSet_10 = sample(negTestingSet10, 10, negTesting_10);
	
	posStratifiedSet_30 = sample(posTestingSet30, 30, posTesting_30);
	negStratifiedSet_30 = sample(negTestingSet30, 30, negTesting_30);

	posStratifiedSet_50 = sample(posTestingSet50, 50, posTesting_50);
	negStratifiedSet_50 = sample(negTestingSet50, 50, negTesting_50);

	stratifiedSet_10 = concatVectors(posStratifiedSet_10, negStratifiedSet_10);
	stratifiedSet_30 = concatVectors(posStratifiedSet_30, negStratifiedSet_30);
	stratifiedSet_50 = concatVectors(posStratifiedSet_50, negStratifiedSet_50);

	testingSet_10 = concatVectors(posTesting_10, negTesting_10);
	testingSet_30 = concatVectors(posTesting_30, negTesting_30);
	testingSet_50 = concatVectors(posTesting_50, negTesting_50);

	
	//findAccuracy(accuracy, totalAccuracy, stratifiedSet, testingSet)
	double accuracy10 = 0, accuracy30 = 0, accuracy50 = 0;
	double totalAccuracy10 = 0, totalAccuracy30 = 0, totalAccuracy50 = 0;
	
	accuracy10 = findAccuracy(stratifiedSet_10, testingSet_10);
	accuracy30 = findAccuracy(stratifiedSet_10, testingSet_10);
	accuracy50 = findAccuracy(stratifiedSet_10, testingSet_10);
	
	printAccuracy(accuracy10, accuracy30, accuracy50);
}

//Converts int to string
std::string convertInt(int x){
	std::string result;
	std::ostringstream convert;
	
	convert << x;
	result = convert.str();
	return result;
}

//Sets all attributes of a person from file
void setPerson(Person &person, int age, std::string workclass, int fnlwgt, 
			std::string education, int educationNum, std::string maritalStatus,
			std::string occupation, std::string relationship, std::string race,
			std::string sex, int capitalGain, int capitalLoss, int hoursPerWeek, 
			std::string nativeCountry, std::string salary){
	person.age = age;
	person.workclass = workclass;
	person.fnlwgt = fnlwgt;
	person.education = education;
	person.educationNum = educationNum;
	person.maritalStatus = maritalStatus;
	person.occupation = occupation;
	person.relationship = relationship;
	person.race = race;
	person.sex = sex;
	person.capitalGain = capitalGain;
	person.capitalLoss = capitalLoss;
	person.hoursPerWeek = hoursPerWeek;
	person.nativeCountry = nativeCountry;	
	person.salary = salary;
}

//Sets up all persons in data set with complete values
void setData(Person person, std::string line, std::vector<Person> &testData){
		line.erase(std::remove(line.begin(), line.end(), ','), line.end());
		
		std::stringstream s(line);
		
		//Nominal attributes
		std::string workclass, education, maritalStatus,
			occupation, relationship, race, sex,
			nativeCountry;
		
		//Continuous attributes
		int age, fnlwgt, educationNum,
		    capitalGain, capitalLoss, hoursPerWeek;
		
		std::string ageStr, fnlwgtStr, educationNumStr,
			capitalGainStr, capitalLossStr, hoursPerWeekStr, salary;
		
		//Read in values into stringstream
		if(s >> age >> workclass >> fnlwgt >> education >> educationNum >>
			maritalStatus >> occupation >> relationship >> race >> sex >> capitalGain >>
			capitalLoss >> hoursPerWeek >> nativeCountry >> salary){
				
				//Convert ints into strings
				ageStr = convertInt(age);
				fnlwgtStr = convertInt(fnlwgt);
				educationNumStr = convertInt(educationNum);
				capitalGainStr = convertInt(capitalGain);
				capitalLossStr = convertInt(capitalLoss);
				hoursPerWeekStr = convertInt(hoursPerWeek);
				
				//Check if values are missing
				if(ageStr == "?" || workclass == "?" ||
					fnlwgtStr == "?" || education == "?" || 
					educationNumStr == "?" || maritalStatus == "?" ||
					occupation == "?" || relationship == "?" ||
					race== "?" || sex == "?" || 
					capitalGainStr == "?" || capitalLossStr == "?" ||
					hoursPerWeekStr == "?" || nativeCountry == "?"){
						
				}
				else{
					setPerson(person, age, workclass, fnlwgt,
						  education, educationNum, maritalStatus, 
						  occupation, relationship, race, sex,
						  capitalGain, capitalLoss, hoursPerWeek,
					          nativeCountry, salary);
					testData.push_back(person);
				}
		}
		
		//printPerson(person);
}

//Reads the file
void readInputFile(std::ifstream &file, std::vector<Person> &testData){
	std::string line;
	while(getline(file,line)){
		Person person;
		setData(person, line, testData);
	}
}

//Prints the usage string
void usageString(){
	std::cout << "Usage: myProgram <input_file>" << std::endl;
}

int main(int argc, char** argv){
	const char *inputfile;
	
	if (argc < 2){
		usageString();
		return EXIT_FAILURE;
	}
	else{
		inputfile = argv[1];
	}
	
	std::ifstream input(inputfile);
	if(!input.is_open()){
		std::cerr << "Error: Data file doesn't exist" << std::endl;
		return EXIT_FAILURE;
	}
	std::vector<Person> testData;
	readInputFile(input, testData);
	stratifiedSample(testData);
	return 1;
}
