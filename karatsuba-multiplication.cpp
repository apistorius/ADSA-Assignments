///***************************************************************************************///
//                                                                                         //
//                      ADSA Assignment 1: Karatsuba Method Calculator                     //
//                          Albert Pistorius (A1716063)                                    //
//                                                                                         //
//  This calculator uses school type methods and the karatsuba method to calculate         //
//  the sums and products of numbers up to 100 digits.                                     //
//  It stores numbers in vectors with the least significant number as the first element    //
//                      So the number 123 would be stored as [3 2 1]                       //
//                      And the number 110100 would be [0 0 1 0 1 1]                       //
//                                                                                         //
///***************************************************************************************///

#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Prints a vector to look like a regular number
void PrintVector(vector<int> vec) {
    for (int i = vec.size()-1; i > -1 ; i--) {
        cout << vec[i];
    }
}

//Removes all leading zero elements in a vector
void RemoveLeadingZeroes(vector<int> *vec) {
    while ((vec->back() == 0) && (vec->size() > 1)) {
        vec->pop_back();
    }
}

//Adds leading zeroes to the shorter vector to equalise the vectors in length
void AddLeadingZeroes(vector<int> *vec1, vector<int> *vec2) {
    int diff = (vec1->size() - vec2->size());       //Calculates the difference between the two vectors lengths
    if (diff > 0) {
        for (int i = 0; i < diff; i++) {
            vec2->push_back(0);                   //Add leading zeroes to vec2
        }
    }
    if (diff < 0) {
        for (int i = 0; i < (0-diff); i++) {
            vec1->push_back(0);                   //Add leading zeroes to vec1
        }
    }
}

//Compares two vectors two find which contains the larger number
bool LargerEqualThan(vector<int> vec1, vector<int> vec2, int length) {
    for (int i = length-1; i > -1 ; i--) {      //Iterates from the most significant number
        if (vec1[i] < vec2[i])
            return false;
        if (vec1[i] > vec2[i])
            return true;
    }
    return true;            //Both numbers are equal
}

//Shifts the vector left by n digits, thereby multiplying it by base^n
void ShiftVectorLeft(vector<int> *vec, int n) {
    if (n == 0)
        return;
    for (int i = 0; i < n; i++) {
        vec->insert(vec->begin(),0);        //Inserts zero in the lowest
    }
}

//Function used for adding two vectors together using the school method
vector<int> SchoolMethodAddition(vector<int> i1, vector<int> i2, int base) {
    vector<int> carry, sum;
    carry.push_back(0);
        for (int i = 0; i < max(i1.size(), i2.size()); i++) {
            if (i1[i] + i2[i] + carry[i] >= base) {         //Handles if a carry is needed
                carry.push_back(1);
                sum.push_back((i1[i] + i2[i] + carry[i] - base));
            } else {                                        //Handles if no carry is needed
                carry.push_back(0);
                sum.push_back((i1[i] + i2[i] + carry[i]));
            }
        }
        if (carry[max(i1.size(), i2.size())] == 1)          //Adds the final carry if needed
            sum.push_back(1);
    return sum;
}

//Function subtracting one number from another using the school method
vector<int> SchoolMethodSubtraction(vector<int> i1, vector<int> i2, int base) {
    vector<int> carry, sum;
    AddLeadingZeroes(&i1, &i2);
    if (!LargerEqualThan(i1, i2, max(i1.size(), i2.size()))) {          //If i2 > i1, recalls the same function but with values swapped around
        sum =  SchoolMethodSubtraction(i2,i1,base);                     // if b > a then a - b = -c, but b - a = c
        sum.back() = -sum.back();                                       //
        return sum;

    } else {
        carry.push_back(0);
        for (int i = 0; i < max(i1.size(), i2.size()); i++) {
            if (i1[i] - i2[i] - carry[i] < 0) {                        //Handles if carry is needed
                sum.push_back(i1[i] - i2[i] - carry[i] + base);
                carry.push_back(1);
            } else {
                sum.push_back(i1[i] - i2[i] - carry[i]);                //Handles if no carry is needed
                carry.push_back(0);
            }
        }
    }
    return sum;
}

//Function to multiply two vectors together using school method
vector<int> SchoolMethodMultiplication(vector<int> i1, vector<int> i2, int base) {
    vector<int> product;
    int unit, carry = 0;

    for (int i = 0; i < (i1.size() + i2.size()); i++)                     //Prepares a blank vector of length: (i1.size()+i2.size())
        product.push_back(0);

    for (int j = 0; j < i2.size(); j++) {
        unit = j;           //Multiplies each iteration by another base^j
        for (int i = 0; i < i1.size(); i++) {
            product[unit] += i1[i] * i2[j];
            if (product[unit] >= base) {
                carry += (product[unit] / base) % base;         //Determines what carry is needed from the product
                product[unit] -= (carry) * base;          //Removes the carry from the original product
            }
            product[unit+1] += carry;      //Brings the carry over to the next unit
            carry = 0;                  //Resets the carry
            unit++;
        }
    }
    return product;
}

//Function to multiply two vectors together using Karatsuba method
vector<int> KaratsubaMulitplication(vector<int> i1, vector<int> i2, int base) {

    vector<int> hi1, hi2, lo1, lo2, z0, z1, z2, a, b, c, output;
    int n;

    RemoveLeadingZeroes(&i1);
    RemoveLeadingZeroes(&i2);
    if ((i1.size() == 1) || (i2.size() == 1)) {             //base case for recursion
        return SchoolMethodMultiplication(i1, i2, base);
    }

    if (i1.size() % 2 == 1)           //Makes all numbers into an even amount of digits
        i1.push_back(0);
    if (i2.size() % 2 == 1)
        i2.push_back(0);
    AddLeadingZeroes(&i1, &i2);

    n = i1.size() / 2;          //Stores the size of each half

    for (int i = 0; i < n; i++) {       //Creates new vectors containing the upper (hi) and lower (lo) halves of each number
        lo1.push_back(i1[i]);
        lo2.push_back(i2[i]);
    }
    for (int i = n; i < 2*n; i++) {
        hi1.push_back(i1[i]);
        hi2.push_back(i2[i]);
    }

    z0 = KaratsubaMulitplication(lo1, lo2, base);       //z0 = (lo1 + lo2)
    a = SchoolMethodAddition(hi1, lo1, base);           //a = (hi1 + lo1)
    b = SchoolMethodAddition(hi2, lo2, base);           //b = (hi2 + lo2)
    z1 = KaratsubaMulitplication(a, b, base);           //z1 = (a * b) = (hi1 + lo1) * (hi2 + lo2)
    z2 = KaratsubaMulitplication(hi1, hi2, base);       //z2 = (hi1 * hi2)

    a.clear();
    b.clear();

    AddLeadingZeroes(&z1, &z0);
    a = SchoolMethodSubtraction(z1, z0, base);      //a = z1 - z0
    AddLeadingZeroes(&a, &z2);
    b = SchoolMethodSubtraction(a, z2, base);       //b = a - z2 = z1 - z0 - z2

    ShiftVectorLeft(&b, n);                 //b = b * base^n
    ShiftVectorLeft(&z2, 2*n);              //z2 = z1 * base^(2n)
    AddLeadingZeroes(&z2, &b);              //Ensures all vectors are
    AddLeadingZeroes(&z2, &z0);             //the same size

    output = SchoolMethodAddition(z2, b, base);         //output = z2 + b
    output = SchoolMethodAddition(output, z0, base);    //output = output + z0 = z2 + b + z0

    RemoveLeadingZeroes(&output);           //Cleans leading zeroes before returning result

    return output;

}

//==============================================================================


int main() {

    vector<int> i1, i2, output;
    vector<string> tokens;
    string input, temp;
    int pos = 0, base, diff, i = 0;

    bool debug = false;        //Toggles debug mode

    getline(cin, input);             //Gets a line of input from user
    while ((pos = input.find(" ")) != input.length()) {      //Retrieves the three integers from the input and stores them in a vector
        tokens.push_back(input.substr(0, pos));
        input.erase(0, pos+1);
        if (pos == -1) break;
    }

    if (tokens.size() != 3) {                    //Error, exits if any inputs are missing
        cout << "Error: Input missing" << endl;
        exit(1);
    }
    if ((tokens[0].length() > 101) || (tokens[1].length() > 101)) {        //Error, exits if either number is more than 100 digits
        cout << "Error: Numbers must be less than 100 digits" << endl;
        exit(1);
    }
    if ((stoi(tokens[2]) < 2) || (stoi(tokens[2]) > 10)) {           //Error, exits if base is out of range
        cout << "Error: Base must be from 2 to 10" << endl;
        exit(1);
    }

    base = stoi(tokens[2]);

    for (int i = tokens[0].size()-1; i > -1; i--) {        //Convert first number into vector i1
        temp = (tokens[0])[i];
        i1.push_back(stoi(temp));
    }
    for (int i = tokens[1].size()-1; i > -1; i--) {        //Convert second number into vector i2
        temp = (tokens[1])[i];
        i2.push_back(stoi(temp));
    }
    AddLeadingZeroes(&i1, &i2);

    if (debug) {
        cout << "number length: " << tokens[0].length() << "\ni1: "; PrintVector(i1);
        cout << "\ni2: "; PrintVector(i2);
        cout << "\nbase: " << base << "\n" << endl;
    }

    output = SchoolMethodAddition(i1, i2, base);
    RemoveLeadingZeroes(&output);
    PrintVector(output); cout << " ";

    output.clear();

    output = KaratsubaMulitplication(i1,i2,base);
    RemoveLeadingZeroes(&output);
    PrintVector(output); cout<<endl;

    return 0;

}
