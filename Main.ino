#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

int emp_count = 0;
int emp_num = 0;

struct employee_acc {
  long employee_id;
  int job_grade;
  String job_title;
  String pen_stat;
  float employee_salary;
};


employee_acc employees[20];

// display the employee details 
int display_employee(int emp_count){


// ignore this bit. Its the custom characters part
  byte up_arrow[8] = {
	0b00100,
	0b01110,
	0b11111,
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b01110
};

byte down_arrow[8] = {
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b11111,
	0b01110,
	0b00100
};

byte pound_symbol[8] = {
  0b00100,
  0b01110,
  0b01000,
  0b11110,
  0b01000,
  0b01000,
  0b11110,
  0b00000
};
// to here

  lcd.clear();

  // Create and display the up arrow
  // ignore this
  lcd.createChar(0, up_arrow);  
  lcd.setCursor(0, 0);       
  //  to this. 
  lcd.write((uint8_t)0);  
  // lcd.write("^"); instead
  // and same thing for the other arrow

  // Create and display the down arrow
  lcd.createChar(1, down_arrow); 
  lcd.setCursor(0, 1);           
  lcd.write((uint8_t)1);

  // display job grade
  lcd.setCursor(1,0);
  lcd.print(employees[emp_count].job_grade);

  // display pension status
  lcd.setCursor(3, 0);
  lcd.print(employees[emp_count].pen_stat);

  // display salary
  // ignore the custom pound symbol its not even an extension
  lcd.createChar(2, pound_symbol);
  lcd.setCursor(8,0);
  lcd.write((uint8_t)2);  
  lcd.print(String(employees[emp_count].employee_salary));

  // display ID
  lcd.setCursor(1,1);
  lcd.print(employees[emp_count].employee_id);

  // display job title
  lcd.setCursor(9,1);
  lcd.print(employees[emp_count].job_title);

  return 0;
}

// ADD function
int add(String account_details){

   // split input string into each component
  String emp_id = account_details.substring(4,11);
  String job_grade = account_details.substring(12,13);
  String job_title = account_details.substring(14);

  // checking if input is valid
  if( (account_details.substring(3,4) == "-") && (account_details.substring(11,12) == "-") && (account_details.substring(13,14) == "-") ){
    Serial.println("valid");

    // checking if inputted id already exists
    for(int i;i<emp_count;i++){
      if (employees[i].employee_id == emp_id.toInt()){
        // should be ERROR: message on serial interface
        Serial.print("ERROR: employee alr exists");
        return 0;
      }
    }

  }else{
    Serial.print("ERROR:, invalid input");
    return 0;

  }

  // create instance of the employee_acc struct and assign data to corresponding variable
  employee_acc employee;
  employee.employee_id = emp_id.toInt();
  employee.job_grade = job_grade.toInt();
  employee.job_title = job_title;
  employee.pen_stat = "PEN";
  employee.employee_salary = 0;

  

  // append struct instance to array
  employees[emp_count] = employee;
  display_employee(emp_count);
  emp_count++;
  emp_num = emp_count;
  
  // checking new employees are being appended to the array correctly
  // for(int i=0;i<emp_count;i++){
  //   Serial.println(employees[i].employee_id);
  //   Serial.println(employees[i].job_grade);
  //   Serial.println(employees[i].job_title);
  //   Serial.println(employees[i].pen_stat);
  // }
  Serial.print("DONE!");
  return 0;
}

// change pension status function
int pst(String pen_details){
  // Serial.print("pst");
  
  // split input string
  String employee_id = pen_details.substring(4,11);
  String pen_stat = pen_details.substring(12);

  // checking if employee exists and if the pen stat is already what its being changed to
  // if valid then change the pen stat
  for(int i;i<emp_count;i++){
    if(employees[i].employee_id == employee_id.toInt()){
      // Serial.println("found");
      Serial.println(employees[i].employee_salary);
      if(employees[i].pen_stat == pen_stat){
        Serial.println("ERROR: inputed pension status is the same as existing status");
      }else if(employees[i].employee_salary == 0){
        Serial.println("ERROR: salary for this employee is £0.00");
        
      }else{
        employees[i].pen_stat = pen_stat;
        emp_num = i;
        display_employee(i);
        Serial.print("DONE!");
      }
    }else{
      Serial.print("ERROR: employee does not exist");
    }
  }
  
  return 0;
}

// change job grade function
int grd(String grade_details){

  // split input string
  String employee_id = grade_details.substring(4,11);
  String job_grade = grade_details.substring(12);

  // check employee exists and if they do then check input is valid and change grade
  for(int i;i<emp_count;i++){
    if(employees[i].employee_id == employee_id.toInt()){
      if( (employees[i].job_grade == job_grade.toInt()) || (employees[i].job_grade > job_grade.toInt()) ){
        Serial.print("ERROR: job grade entered is the same or lower");
      }else{
        employees[i].job_grade = job_grade.toInt();
        emp_num = i;
        display_employee(i);
        Serial.print("DONE!");
      }
    }else{
      Serial.print("employee does not exist");
    }
  }

  
  return 0;
}

// change salary function
int sal(String salary_details){

  // split input string
  String employee_id = salary_details.substring(4,11);
  String employee_salary = salary_details.substring(12);

  // convert salary to a float and round it to two dp
  float salary = atof(employee_salary.c_str());
  salary = round(salary * 100) / 100;

  if( (salary < 0) || (salary > 100000) ){
    Serial.println("ERROR: salary out of range");
    return 0;
  }

  // check if employee exists and then change salary
  for(int i;i<emp_count;i++){
    if(employees[i].employee_id == employee_id.toInt()){
        employees[i].employee_salary = salary;
        emp_num = i;
        display_employee(i);
        Serial.print("DONE!");
    return 0;
    }
  }

  Serial.println("ERROR: employee does not exist");
  return 0;
}

// change job title function
int cjt(String job_title_details){

  // split input string
  String employee_id = job_title_details.substring(4,11);
  String job_title= job_title_details.substring(12);

  // check if input is valid
  if( (job_title.length() < 3) || (job_title.length() >  17) || (job_title.indexOf("-") != -1)){
    Serial.print("ERROR: job title length is not the right size");
  }else{
    // check if employee exists 
    for(int i;i<emp_count;i++){
      if(employees[i].employee_id == employee_id.toInt()){
          employees[i].job_title = job_title;
          emp_num = i;
          display_employee(i);
          Serial.print("DONE!");
      }else{
        Serial.print("employee does not exist");
      }
    }
  }

  
  return 0;
}

// delete employee record function
int del(String delete_details){

  // split string input
  String employee_id = delete_details.substring(4);

  // check if employee exist and then delete them
  for(int i;i<emp_count;i++){
    if(employees[i].employee_id == employee_id.toInt()){
      // move every struct one to the left in the array starting from the struct after the struct to be deleted
      for (int j = i; j < emp_count - 1; j++) {
        employees[j] = employees[j + 1];
    }
      // Decrease the size of the array
      emp_count--;
      display_employee(i);
      Serial.print("DONE!");
    }else{
      Serial.print("employee does not exist");
    }
  }
  
  return 0;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // configure lcd and set backlight to yellow
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.setBacklight(3);

  // synchronisation phase. Dont know if I got the "NL" and "CR" stuff right 
  String beginInput;
  unsigned long lastPrintTime = 0;
  while (beginInput != "BEGIN"){
    if(Serial.available() > 0){
      beginInput = Serial.readString();
      if( (beginInput == "\n") || (beginInput == "\r") ){
        Serial.print("ERROR: no newline or carraige return");
      }
    }
    unsigned long time = millis();
    if( (time % 2000 == 0) && (time != lastPrintTime) ){
      Serial.print("R");
      lastPrintTime = time;
    }
  }

  // print "BASIC" and set backlight to white
  lcd.clear();
  lcd.print("BASIC");
  Serial.println("BASIC");
  lcd.setBacklight(7);
  delay(500);
  lcd.clear();

  // test employee
  // employee_acc test_employee;
  // test_employee.employee_id = 1000000;
  // test_employee.job_grade = 1;
  // test_employee.job_title = "teacher";
  // test_employee.pen_stat = "PEN";
  // test_employee.employee_salary = 1000.00;

  // employees[0] = test_employee;

  
}

// used for the attempt on case and switch
// converts string inputs to values from the enum
// Mode string_to_state(const String& str){
//   if (str == "ADD") return ADD;
//   else if (str == "PST") return PST;
//   else if (str == "GRD") return GRD;
//   else if (str == "SAL") return SAL;
//   else if (str == "CJT") return CJT;
//   else if (str == "DEL") return DEL;
//   else Serial.print("ERROR: invalid input");
// }
 

void loop() {
  // put your main code here, to run repeatedly:
  
  // recieve input from user
  String user_input = Serial.readString();
  String func_select = user_input.substring(0,3);

  // revieve button input from user
  uint8_t buttons = lcd.readButtons();

  // run the corresponding function
  if(func_select == "ADD"){
    add(user_input);
  }else if(func_select == "PST"){
    pst(user_input);
  }else if(func_select == "GRD"){
    grd(user_input);
  }else if(func_select == "SAL"){
    sal(user_input);
  }else if(func_select == "CJT"){
    cjt(user_input);
  }else if(func_select == "DEL"){
    del(user_input);
  }

  // show previous employee when up button is pressed
  if (buttons & BUTTON_UP) {
    Serial.print("UP");
    // ignore this bit
    if (emp_num == 0){
      emp_num = emp_count;
    =
    }else{
      emp_num--;
    }
    // to this bit
    Serial.println(emp_num);
    display_employee(emp_num);
    // show next employee if down button is pressed
  }else if (buttons & BUTTON_DOWN) {
    Serial.print("DOWN");
    // ignore this bit
    if (emp_num == 20){
      emp_num = 0;
    }else{
      emp_num++;
    }
    Serial.print(emp_num);
    display_employee(emp_num);
  }
  // to this bit
  else if (buttons & BUTTON_SELECT){
    lcd.setBacklight(5);
    lcd.print("F420588");
  }

// an unsuccessful attempt to use switch case
  // String user_input = Serial.readString().substring(0,3);

  // Mode current_mode = string_to_state(user_input);

  // switch (current_mode){
  //   case ADD:
  //   // add(user_input);
  //   Serial.print("add test succcc")
  // }

  


}
