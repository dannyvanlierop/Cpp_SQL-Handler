
#if defined(ARDUINO_ESP32_DEV)
    #include <WiFi.h>
    #include <WiFiClient.h>
#elif defined (ARDUINO_ESP8266_WEMOS_D1MINI)
    #include <ESP8266WiFi.h>
#endif

#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#include "./token.h"

WiFiClient client;
MySQL_Connection conn((Client *)&client);

int loopCounter = 1;
bool bDebugSQL = true;

//For pretty console print
int iQueryStep = 1;
const int iQueryStepIncrease = 2;
String StepReturn(int iIncrease = 0){
    
    iQueryStep = iQueryStep + iIncrease;
    String sRetval = "";

    for(int iPos = 0; iPos <= iQueryStep; iPos++)
    {
        sRetval += "\t";
    }
    return sRetval;
}
String setStringLength(String stringInput, String charInput, int stringLengthNew, int intPlacement = 0){

    String stringExtendPrefix = "";
    String stringExtendSuffix = "";
    
    int stringLengthOld = stringInput.length();  
    int stringLengthDif = (stringLengthNew - stringLengthOld?stringLengthNew - stringLengthOld:0);

    int intPrefixLength = 0;
    int intSuffixLength = 0;

    switch(intPlacement) 
    {
        case 0: 
            intSuffixLength = stringLengthDif;
            break;
        case 1: 
            intPrefixLength = stringLengthDif;
            break;
        case 2: 
            intPrefixLength = stringLengthDif/2;
            intSuffixLength = stringLengthDif/2;
            if (stringInput.length() % 2 != 0){
                intSuffixLength++;
            }
            break;
    };
    
    if (intPlacement != 0 ) //if not suffix only
    {
        for (int pos = 0; pos <= intPrefixLength; pos++)
        {
          stringExtendPrefix += charInput;
        };
    }

    if (intPlacement != 1 ) //if not prefix only
    {
        for (int pos = 0; pos <= intSuffixLength; pos++)
        {
          stringExtendSuffix += charInput;
        };
    }

    return stringExtendPrefix + stringInput + stringExtendSuffix;
}

/*WIFI*//////////////////////////////////////////////////////////////////
bool WifiStatus(){ return (WiFi.status() == WL_CONNECTED); };
void WifiConnect()
{
    Serial.print(WiFi.begin(mySQLWiFiSSID, mySQLWiFiPass));
    while (!WifiStatus())
    {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println(WiFi.localIP());
}

/*MYSQL*//////////////////////////////////////////////////////////////////
bool MysqlClientStatus()
{
    return conn.connected();
};
bool MysqlClientConnect()
{
    return conn.connect(mySQLServerIP, mySQLServerPort, mySQLUserName, mySQLUserPass);
    }
String MysqlClientDisconnect() { 
    conn.close();
    return MysqlClientStatus() ? "MySQL Disconnect failed. " : "MySQL Disconnected.";
};
void MysqlClientReconnect(){
    MysqlClientDisconnect(); delay(10); MysqlClientConnect();
};

/*GET*////////////////////////////////////////////////////////////////////
String MysqlQueryGetDatabase(String sDB)
{
    return "SHOW DATABASES LIKE '" + sDB + "';";
};
String MysqlQueryGetTable(String sDB, String sTable){
    return "SELECT TABLE_NAME FROM information_schema.tables WHERE TABLE_SCHEMA ='" + sDB + "' AND TABLE_NAME ='" + sTable + "' LIMIT 1;";
};
String MysqlQueryGetColumn(String sDB, String sTable, String sColumn)
{
    return "SELECT COLUMN_NAME FROM information_schema.columns WHERE TABLE_SCHEMA ='" + sDB + "' AND TABLE_NAME ='" + sTable + "' AND COLUMN_NAME = '" + sColumn + "' LIMIT 1;";
};
String MysqlQueryGetColumns(String sDB, String sTable, String sColumn)
{
    return "SELECT COLUMN_NAME FROM information_schema.columns WHERE TABLE_SCHEMA = '" + sDB + "'";
};

/*GET_COLUMN_VALUE_FIRST_SHOW_ONE*////////////////////////////////////////////////////////////////////
String MysqlQueryGetColumnValueFirstShowOne(String sDB, String sTable, String sColumn)
{
    return "SELECT " + sColumn + " FROM " + sDB + "." + sTable + " WHERE `" + sColumn + "` IS NOT NULL ORDER BY Id ASC LIMIT 1";
};

/*GET_COLUMN_VALUE_LAST_SHOW_ONE*/ ///////////////////////////////////////////////////////////////////
String MysqlQueryGetColumnValueLastShowOne(String sDB, String sTable, String sColumn)
{
    return "SELECT " + sColumn + " FROM " + sDB + "." + sTable + " WHERE `" + sColumn + "` IS NOT NULL ORDER BY Id DESC LIMIT 1";
};

/*GET_COLUMN_VALUE_FIRST_SHOW_ROW*////////////////////////////////////////////////////////////////////
String MysqlQueryGetColumnValueFirstShowRow(String sDB, String sTable, String sColumn)
{
    return "SELECT * FROM " + sDB + "." + sTable + " WHERE `" + sColumn + "` IS NOT NULL ORDER BY Id ASC LIMIT 1";
};

/*GET_COLUMN_VALUE_LAST_SHOW_ROW*////////////////////////////////////////////////////////////////////
String MysqlQueryGetColumnValueLastShowRow(String sDB, String sTable, String sColumn)
{
    return "SELECT * FROM " + sDB + "." + sTable + " WHERE `" + sColumn + "` IS NOT NULL ORDER BY Id DESC LIMIT 1";
};

/*CREATE*////////////////////////////////////////////////////////////////////
String MysqlQueryCreateDatabase(String sDB)
{
    return "CREATE DATABASE IF NOT EXISTS " + sDB + ";";
};
String MysqlQueryCreateTable(String sDB, String sTable)
{
    return "USE " + sDB + "; CREATE TABLE IF NOT EXISTS `" + sTable + "` (  `Id` int(11) NOT NULL, `Timestamp` timestamp NOT NULL DEFAULT current_timestamp()) ENGINE=InnoDB DEFAULT CHARSET=utf8; TRUNCATE TABLE `" + sTable + "`; ALTER TABLE `" + sTable + "` ADD PRIMARY KEY (`Id`) USING BTREE;" + "ALTER TABLE `" + sTable + "` MODIFY `Id` int(11) NOT NULL AUTO_INCREMENT;";
    };
String MysqlQueryCreateColumn(String sDB, String sTable, String sColumn)
{
    return "USE " + sDB + "; ALTER TABLE " + sTable + " ADD COLUMN " + sColumn + " VARCHAR(15);";
};

/*DELETE*////////////////////////////////////////////////////////////////////
String MysqlQueryDeleteDatabase(String sDB)
{
    return "USE " + sDB + "; DROP DATABASE IF EXISTS " + sDB + ";";
};
String MysqlQueryDeleteTable(String sDB, String sTable)
{
    return "USE " + sDB + "; DROP TABLE IF EXISTS " + sDB + "." + sTable + ";";
};
String MysqlQueryDeleteColumn(String sDB, String sTable, String sColumn)
{
    return "USE " + sDB + "; ALTER TABLE " + sTable + " DROP COLUMN IF EXISTS " + sColumn + ";";
};

/*UPDATE*////////////////////////////////////////////////////////////////////
String MysqlQueryUpdateValue(String sDB, String sTable, String sColumn, String sInOutValue)
{
    return "UPDATE " + sDB + "." + sTable + " SET " + sColumn + "='" + sInOutValue + "' WHERE Id = 1;";
}; // Query(MySQL) : UPDATE DatabaseName.TableName SET ColumnName1='testString';
/*INSERT*/
String MysqlQueryInsertValue(String sDB, String sTable, String sColumn, String sInOutValue)
{
    return "INSERT INTO " + sDB + "." + sTable + " (" + sColumn + ") VALUES ('" + sInOutValue + "');";
};// Query(MySQL) : INSERT INTO DatabaseName.TableName (ColumnName1) VALUES ('testString');

//TODO:
/*COUNT*////////////////////////////////////////////////////////////////////
//ONDELETE -> DROP Table when its the last column - count columns in table
//ONDELETE -> DROP database when its the last Table - count tables in database

//Convert string to char array pointer
char* strTooChar(String sQuery)
{
    int sLength = sQuery.length() + 1;                                                                                                      //The string input length.
    char charQuery[sLength];                                                                                                                //A new char array with the length of inputString.
    sQuery.toCharArray(charQuery, sLength); 
}
//The MYSQL cursor
MySQL_Cursor * MysqlQueryCursor(String sQuery)
{
    //Serial.println("Mysql Query : ( " + sQuery + " )");                                                                                   //The string input.
    int sLength = sQuery.length() + 1;                                                                                                      //The string input length.
    char charQuery[sLength];                                                                                                                //A new char array with the length of inputString.
    sQuery.toCharArray(charQuery, sLength);                                                                                                 //Convert String to char array.
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);                                                                                        //Initiate the MySQL_Cursor class instance.
    cur_mem->execute(charQuery);                                                                                                            //Execute the query.
    return cur_mem;                                                                                                                         //Return the query results
}

//The MySQL query execution
String * MysqlQueryDo(String sStatement, String sDB, String sTable = "", String sColumn = "", String sInOutValue = "")
{  
    String * pInOutValue = &sInOutValue;   
    
    //General options             //DB options        //Table options        //Column options        //Item-value options
                             bool bDb        = false, bTable        = false, bColumn        = false, bValue        = false; //Type options
    bool bGet            = false, bDb_Get    = false, bTable_Get    = false, bColumn_Get    = false, bValue_Get    = false; //Get options
    bool bCreate         = false, bDb_Create = false, bTable_Create = false, bColumn_Create = false, bValue_Create = false; //Create options
    bool bDelete         = false, bDb_Delete = false, bTable_Delete = false, bColumn_Delete = false, bValue_Delete = false; //Delete option
    bool bExists         = false, bDb_Exists = false, bTable_Exists = false, bColumn_Exists = false, bValue_Exists = false; //Exist options  //not used in MysqlQueryDo()
    bool bInsert         = false;                                                                                           //Insert options
    bool bUpdate         = false;                                                                                           //Get multi values
    bool bValue_Multi    = false;                                                                                           //Update options                                                                                         //Update options
      
    bool bIterateRow = true;     //defines what query results to skip or fetch.
    bool bIterateCol = false;    //defines what query results to skip or fetch.
  String sQuery = "";            //String variable for our query.

    //Get action and iteration type
         if( sStatement == "GET_DATABASE"                       ){ bDb_Get        =  true; sQuery = MysqlQueryGetDatabase(sDB);                                    } //Get database(sDB) and returns databaseName.
    else if( sStatement == "GET_TABLE"                          ){ bTable_Get     =  true; sQuery = MysqlQueryGetTable(sDB,sTable);                                } //Get table(sTable) , in database(sDB) and returns tableName.
    else if( sStatement == "GET_COLUMN"                         ){ bColumn_Get    =  true; sQuery = MysqlQueryGetColumn(sDB,sTable,sColumn);                       } //Get column(sColumn) and returns columnName.
    else if( sStatement == "CREATE_DATABASE"                    ){ bDb_Create     =  true; sQuery = MysqlQueryCreateDatabase(sDB);                                 } //Create a new database(sDB) and returns databaseName.
    else if( sStatement == "CREATE_TABLE"                       ){ bTable_Create  =  true; sQuery = MysqlQueryCreateTable(sDB,sTable);                             } //Create a new table(sTable), in database(sDB) and returns tableName.    
    else if( sStatement == "CREATE_COLUMN"                      ){ bColumn_Create =  true; sQuery = MysqlQueryCreateColumn(sDB,sTable,sColumn);                    } //Create a new column(sColumn), in database(sDB), on table(sTable) and returns columnName.
    else if( sStatement == "DELETE_DATABASE"                    ){ bDb_Delete     =  true; sQuery = MysqlQueryDeleteDatabase(sDB);                                 } //Delete database(sDB) and returns databaseName.
    else if( sStatement == "DELETE_TABLE"                       ){ bTable_Delete  =  true; sQuery = MysqlQueryDeleteTable(sDB,sTable);                             } //Delete table(sTable), in database(sDB) and returns tableName.
    else if( sStatement == "DELETE_COLUMN"                      ){ bColumn_Delete =  true; sQuery = MysqlQueryDeleteColumn(sDB,sTable,sColumn);                    } //Delete column(sColumn), in database(sDB), on table(sTable) and returns columnName.
    else if( sStatement == "GET_COLUMN_VALUE_FIRST_SHOW_ONE"    ){ bValue_Get     =  true; sQuery = MysqlQueryGetColumnValueFirstShowOne(sDB, sTable, sColumn);    } //Gets the first value from column(sColumn) on table(sTable) in database(sDB), and returns one value.
    else if( sStatement == "GET_COLUMN_VALUE_LAST_SHOW_ONE"     ){ bValue_Get     =  true; sQuery = MysqlQueryGetColumnValueLastShowOne(sDB, sTable, sColumn);     } //Gets the  last value from column(sColumn) on table(sTable) in database(sDB), and returns one value.
    else if( sStatement == "GET_COLUMN_VALUE_FIRST_SHOW_ROW"    ){ bValue_Multi   =  true; sQuery = MysqlQueryGetColumnValueFirstShowRow(sDB, sTable, sColumn);    } //Gets the first value from column(sColumn) on table(sTable) in database(sDB), and returns the whole row.
    else if( sStatement == "GET_COLUMN_VALUE_LAST_SHOW_ROW"     ){ bValue_Multi   =  true; sQuery = MysqlQueryGetColumnValueLastShowRow(sDB, sTable, sColumn);     } //Gets the  last value from column(sColumn) on table(sTable) in database(sDB), and returns the whole row.
    else if( sStatement == "INSERT_VALUE"                       ){ bInsert        =  true; sQuery = MysqlQueryInsertValue(sDB, sTable, sColumn, sInOutValue);      } //Insert value(sInOutValue) to column(sColumn) on table(sTable) in database(sDB), and returns.
    else if( sStatement == "UPDATE_VALUE"                       ){ bUpdate        =  true; sQuery = MysqlQueryUpdateValue(sDB, sTable, sColumn, sInOutValue);      } //Update value(sInOutValue) on column(sColumn) on table(sTable) in database(sDB), and returns.      

    //Get item action
    bGet    = ( bDb_Get     || bTable_Get     || bColumn_Get    || bValue_Get   );          //
    bCreate = ( bDb_Create  || bTable_Create  || bColumn_Create || bValue_Create);          //
    bDelete = ( bDb_Delete  || bTable_Delete  || bColumn_Delete || bValue_Delete);          //
    bExists = ( bDb_Exists  || bTable_Exists  || bColumn_Exists || bValue_Exists);          //

    //Get item type
    bDb     = ( bDb     || bDb_Get     || bDb_Create     || bDb_Delete     );                  //
    bTable  = ( bTable  || bTable_Get  || bTable_Create  || bTable_Delete  );                  //
    bColumn = ( bColumn || bColumn_Get || bColumn_Create || bColumn_Delete );                  //
    bValue  = ( bValue  || bValue_Get  || bValue_Create  || bValue_Delete  || bValue_Multi );  //
    
    //Combines/Corrections here
    bValue_Get  = ( bValue_Get || bValue_Multi);        //Set bValue_Get when we have any type of get
    bIterateCol = ( bIterateCol || bValue_Get);         //Iterate when we need to get a value for return
    bIterateRow = ( bIterateRow && !bCreate );          //Iterate rows by default, only skip on create db,table, column
    bColumn_Get = ( bColumn_Get || bValue_Get );        //Check columns existence before fetching

    while( !WifiStatus() ) WifiConnect();               //Wait for the connection.
    while( !MysqlClientStatus() ) MysqlClientConnect(); //Wait for the connection.

    if(bDebugSQL){
        Serial.println();
        Serial.println(StepReturn() + "│");
        Serial.print(StepReturn() + "└─► " + sStatement);
        Serial.print(" ◄─ INPUT ◄── "  + sDB);
        Serial.print(String("") != sTable?" ◄── " + sTable:"");
        Serial.print(String("") != sColumn?" ◄── " + sColumn:"");
        Serial.println(String("") != sInOutValue?" ◄── " + sInOutValue:"");
    }  
   
    MySQL_Cursor *cur_mem  = MysqlQueryCursor(sQuery);//End Query 1 //Initiate the query class instance, and run the first query

    if(bIterateCol||bIterateRow)    //To skip iteration for some items
    {           
        //Serial.print(" -> iterate ");                                                                                            
        column_names *cols = cur_mem->get_columns();    //Fetch the columns.
        if(bIterateCol)
        {
            if(bDebugSQL)Serial.print(StepReturn() + "\t└┐\n");
            for (int iCol = 0; iCol < cols->num_fields; iCol++) {   //iterate through columns.               
                if(bDebugSQL)Serial.print(StepReturn() + "\t ├─► col" + String(iCol) + " = ");                
                if(bDebugSQL)Serial.println(cols->fields[iCol]->name);
            };
            if(bDebugSQL)Serial.print(StepReturn() + "\t┌┘\n");
        };
        row_values *row = NULL;
        if(bIterateRow)
        {
            do {    //Read the rows and print them   
                row = cur_mem->get_next_row();
                if (row != NULL) {  //iterate through rows while we got results.
                    if(bDebugSQL)Serial.print(StepReturn() + "\t└┐\n");
                    for (int iRow = 0; iRow < cols->num_fields; iRow++) {        
                        String sInputString  = String(row->values[iRow]);
                        if(bDebugSQL)Serial.println(StepReturn() + "\t ├─► row" + String(iRow) + " = " + sInputString);  //Print row number and content

                        if ( bDb ) {                        if( sInputString == sDB ){        Serial.println("\n sInOutValue: " + sDB); sInOutValue = sDB;       break; } }                          
                        else if ( bTable ) {                if( sInputString == sTable ){     Serial.println("\n sInOutValue: " + sTable); sInOutValue = sTable;    break; } }
                        else if ( bColumn && !bValue_Get) { if( sInputString == sColumn ){    sInOutValue = sColumn;   break; } }
                        else if ( bValue_Get ) {            if( sInputString != NULL ){ //Dont add null values
                                                                if(!bValue_Multi){//Get a single value
                                                                    sInOutValue = sInputString;
                                                                    break; 
                                                                } else {    //Get multiple values
                                                                    sInOutValue += sInputString;
                                                                    if( (iRow + 1) < cols->num_fields) {    //Add comma to separate values.
                                                                        sInOutValue += ",";
                                                                    } else {
                                                                        break;
                                                                    }
                                                                }
                                                            }
                        }
                    };
                };
            } while (row != NULL);  //Read till we have all the rows.
            if(bDebugSQL)Serial.print(StepReturn() + "\t┌┘\n");
        }
    }
    
    delete cur_mem;//Deleting the cursor also frees up memory used.

    MysqlClientDisconnect();//Disconnect from MySql Server.

    if(bDebugSQL){
        Serial.print(StepReturn(1) + "└─► OUTPUT ─► ");

        if (bValue){            Serial.print(sInOutValue != ""?"value " + sInOutValue + " found":"Value not found");        } 
        else if(bInsert){       Serial.print(sInOutValue != ""?"value " + sInOutValue + " Inserted":"Insert not found");      } 
        else if(bUpdate){       Serial.print(sInOutValue != ""?"value " + sInOutValue + " Updated":"Update not found");      } 
        else if(bDb)
        {
                 if(bGet){      Serial.print(sInOutValue == sDB?"database " + sInOutValue + " found":"ERROR: database not found"); }
            else if(bCreate){   Serial.print(sInOutValue == ""?"database " + sDB + " created":"ERROR: database not created"); }
            else if(bDelete){   Serial.print(sInOutValue == ""?"database " + sInOutValue + " deleted":"ERROR: database not deleted"); }
        }
        else if(bTable)
        {
                 if(bGet){      Serial.print(sInOutValue == sTable?"table " + sInOutValue + " found":"ERROR: table not found"); }
            else if(bCreate){   Serial.print(sInOutValue == ""?"table " + sTable + " created":"ERROR: table not created"); }
            else if(bDelete){   Serial.print(sInOutValue == ""?"table " + sInOutValue + " deleted":"ERROR: table not deleted"); }
        }
        else if(bColumn)
        { 
            if(bGet){           Serial.print(sInOutValue == sColumn?"column " + sInOutValue + " found":"ERROR: column not found"); }
            else if(bCreate){   Serial.print(sInOutValue == ""?"column " + sColumn + " created":"ERROR: column not created"); }
            else if(bDelete){   Serial.print(sInOutValue == ""?"column " + sInOutValue + " deleted":"ERROR: column not deleted"); }

        };
        iQueryStep++;
    }
    return pInOutValue; //Return the value as pointer.
};

//The MySQL query (Pre)execution
String * MysqlQuery(String sStatement, String sDB, String sTable = "", String sColumn = "", String sInOutValue = "")
{    
    String * pInOutValue = &sInOutValue;   
    
    //General options             //DB options        //Table options        //Column options        //Item-value options
                             bool bDb        = false, bTable        = false, bColumn        = false, bValue        = false; //Type options
    bool bGet            = false, bDb_Get    = false, bTable_Get    = false, bColumn_Get    = false, bValue_Get    = false; //Get options
    bool bCreate         = false, bDb_Create = false, bTable_Create = false, bColumn_Create = false, bValue_Create = false; //Create options
    bool bDelete         = false, bDb_Delete = false, bTable_Delete = false, bColumn_Delete = false, bValue_Delete = false; //Delete option
    bool bExists         = false, bDb_Exists = false, bTable_Exists = false, bColumn_Exists = false, bValue_Exists = false; //Exist options  //not used in MysqlQueryDo()
    bool bInsert         = false;                                                                                           //Insert options
    bool bUpdate         = false;                                                                                           //Get multi values
    bool bValue_Multi    = false;                                                                                           //Update options 

    if(bDebugSQL)Serial.print("\nRequest:" + setStringLength(sStatement, " ", 31));

    //Get action and item type
         if( sStatement == "GET_DATABASE"                       ){ bDb_Get        = true; }
    else if( sStatement == "GET_TABLE"                          ){ bTable_Get     = true; }
    else if( sStatement == "GET_COLUMN"                         ){ bColumn_Get    = true; }  
    else if( sStatement == "CREATE_DATABASE"                    ){ bDb_Create     = true; }
    else if( sStatement == "CREATE_TABLE"                       ){ bTable_Create  = true; }    
    else if( sStatement == "CREATE_COLUMN"                      ){ bColumn_Create = true; }
    else if( sStatement == "DELETE_DATABASE"                    ){ bDb_Delete     = true; }
    else if( sStatement == "DELETE_TABLE"                       ){ bTable_Delete  = true; }
    else if( sStatement == "DELETE_COLUMN"                      ){ bColumn_Delete = true; }
    else if( sStatement == "INSERT_VALUE"                       ){ bInsert        = true; }
    else if( sStatement == "UPDATE_VALUE"                       ){ bUpdate        = true; }
    else if( sStatement.startsWith("GET_COLUMN_VALUE")          ){ bValue_Get     = true; if( sStatement.endsWith("SHOW_ROW") )bValue_Multi = true; }
    
    //Get item type
    bDb     = ( bDb     || bDb_Get     || bDb_Create     || bDb_Delete     );                  //
    bTable  = ( bTable  || bTable_Get  || bTable_Create  || bTable_Delete  );                  //
    bColumn = ( bColumn || bColumn_Get || bColumn_Create || bColumn_Delete );                  //
    bValue  = ( bValue  || bValue_Get  || bValue_Create  || bValue_Delete  || bValue_Multi );  //
   
    String sGetResult;      //check item(root) existence

    //Check if the item location exists
    iQueryStep = 1;//reset!!    
    if(bDb)          {  sGetResult = *MysqlQueryDo("GET_DATABASE", sDB);                bDb_Exists     = (sGetResult != NULL); }
    else if(bTable)  {  sGetResult = *MysqlQueryDo("GET_TABLE", sDB, sTable);           bTable_Exists  = (sGetResult != NULL); }
    else if(bColumn || 
            bInsert || 
            bUpdate ||
            bValue_Get ){  sGetResult = *MysqlQueryDo("GET_COLUMN", sDB, sTable, sColumn); bColumn_Exists = (sGetResult != NULL); }

    if(bValue_Get && !bColumn_Exists){//Location of value doesnt exist.
        if(bDebugSQL)Serial.print(" ─► Skip...");
        sInOutValue = "";
        return pInOutValue; 
    }

    //Get item action
    bGet    = ( bDb_Get     || bTable_Get     || bColumn_Get    || bValue_Get   );          //
    bCreate = ( bDb_Create  || bTable_Create  || bColumn_Create || bValue_Create);          //
    bDelete = ( bDb_Delete  || bTable_Delete  || bColumn_Delete || bValue_Delete);          //
    bExists = ( bDb_Exists  || bTable_Exists  || bColumn_Exists || bValue_Exists);          //

    //Done preset, time for some action
    if(bCreate || bInsert || bUpdate)
    {
        //sQueryAction = "CREATE";

        if(!bExists)//Item does not exist yet.
        {
            if(bDb){//DATABASE - Check database, table and column requests, if database exists, and dont double check database existence.
                MysqlQueryDo("CREATE_DATABASE", sDB);//database doesnt exist at this moment, create Immediately
            } 
            else if( *MysqlQueryDo("GET_DATABASE" , sDB) == "")//Unsure if databse exist, check existence.
            {
                MysqlQueryDo("CREATE_DATABASE", sDB);//not found, create database.
            };//Database must be ok here.

            if (!bDb)//TABLE - Check table, on table and column requests.
            {
                if( bTable_Create && !bExists || *MysqlQueryDo("GET_TABLE" , sDB, sTable) == "")//Check if table exists.
                {
                    MysqlQueryDo("CREATE_TABLE", sDB, sTable);//not found, create table.
                }
            };//Table must be ok here.

            if (!bDb && !bTable)//COLUMN - Check column.
            {
                if(*MysqlQueryDo("GET_COLUMN" , sDB, sTable, sColumn) == "")//Check if column exists.
                {
                    MysqlQueryDo("CREATE_COLUMN", sDB, sTable, sColumn);//Not found, create column.
                    MysqlQueryDo("INSERT_VALUE", sDB, sTable, sColumn, "TEMPInitValue");//Create init value 
                }
            };//Column must be ok here.          
        }     
        else {
            if(bDebugSQL && bCreate)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Skip Create...");
            if(bDebugSQL && bInsert)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Do Insert...");
            if(bDebugSQL && bUpdate)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Do Update...");
        }
       
        if(bInsert){ MysqlQueryDo("INSERT_VALUE", sDB, sTable, sColumn, sInOutValue); }//Add value to column
        if(bUpdate){ MysqlQueryDo("UPDATE_VALUE", sDB, sTable, sColumn, sInOutValue); }//Update value at id=1 on column
    } 
    else if(bDelete)
    {
        if(bExists){//Delete database,table,column
            if(bDb)         { MysqlQueryDo("DELETE_DATABASE", sDB); }
            else if(bTable) { MysqlQueryDo("DELETE_TABLE", sDB, sTable); }
            else if(bColumn){ MysqlQueryDo("DELETE_COLUMN", sDB, sTable , sColumn); };
        } else {
            if(bDebugSQL)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Skip delete...");//Skip deleting non existing items
        }
    }  
    if(bGet){

        if (bColumn_Exists) { 
            sInOutValue = *MysqlQueryDo(sStatement, sDB, sTable, sColumn, sInOutValue); }//Get value from column
        } else {
            sInOutValue = "";//Location doesnt exist, return null.
        }

    return pInOutValue; //Return the value as pointer.
}

//The main loop
void loop()
{
    int r = rand() % 15;

    r = 2;

    String hostname = "testing";
    switch(r){

        //case 1:  MysqlQuery("CREATE_DATABASE", "iot"); break;
        //case 2:  MysqlQuery("CREATE_TABLE", "iot", "table5"); break;
        //case 2:  MysqlQuery("CREATE_COLUMN", "iot", "table5", "column5"); break;
        //case 4:  MysqlQuery("GET_DATABASE", "iot"); break;
        //case 5:  MysqlQuery("GET_TABLE", "iot", "table5"); break;
        //case 6:  MysqlQuery("GET_COLUMN", "iot", "table5", "column5"); break;
        case 2:  MysqlQuery("INSERT_VALUE", "iot", "table5", "column5", String(loopCounter)); break;
        //case 8:  MysqlQuery("UPDATE_VALUE", "iot", "table5", "column5", String(loopCounter)); break;
        //case 9:  MysqlQuery("GET_COLUMN_VALUE_FIRST_SHOW_ONE", "iot", "table5", "column5"); break;
        //case 10: MysqlQuery("GET_COLUMN_VALUE_LAST_SHOW_ONE", "iot", "table5", "column5"); break;
        //case 11: MysqlQuery("GET_COLUMN_VALUE_FIRST_SHOW_ROW", "iot", "table5", "column5"); break;
        //case 12: MysqlQuery("GET_COLUMN_VALUE_LAST_SHOW_ROW", "iot", "table5", "column5"); break;
        //case 13: MysqlQuery("DELETE_COLUMN", "iot", "table5", "column5"); break;
        //case 2: MysqlQuery("DELETE_TABLE", "iot", "table5"); ; break;
        //case 15: MysqlQuery("DELETE_DATABASE", "iot"); break;
    }           

    //MysqlQuery("INSERT_VALUE", "iot", *getItemSvalue("hostname"), "column5", String(longCounterLoopCurrent));
    //MysqlQuery("UPDATE_VALUE", "iot", *getItemSvalue("hostname"), "column5", String(longCounterLoopCurrent));
    Serial.print("\n.............................................................. loop:" + String(loopCounter++) + " ..............................................................");   
    delay(3000); //Slow down between loops
    iQueryStep = 1;//Reset
};

//Global setup
void setup() {
    Serial.begin(250000);   //Start serial monitor.
    while (!Serial);        //Wait for serial port to connect.
    Serial.println();       //Start the console on a new line.
    WifiConnect();          //Connect the WifiClient.
};

