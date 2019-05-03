
#if defined(ARDUINO_ESP32_DEV)
    #include <WiFi.h>
    #include <WiFiClient.h>
#elif defined (ARDUINO_ESP8266_WEMOS_D1MINI)
    #include <ESP8266WiFi.h>
#endif

#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

IPAddress mySQLServerIP = IPAddress(0, 0, 0, 1);
int mySQLServerPort = 0;
char* mySQLUserName = ""; 
char* mySQLUserPass = "";
char* mySQLWiFiSSID = ""; 
char* mySQLWiFiPass = "";

WiFiClient client;
MySQL_Connection conn((Client *)&client);
int loopCounter = 1;

bool bDebug = true;
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
//String MysqlQueryGetDatabase(String sDB)
//{
//    return "SHOW DATABASES LIKE '" + sDB + "';";
//};
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
String MysqlQueryCreateTable(String sDB, String sTable
){
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

/*COUNT*////////////////////////////////////////////////////////////////////


//ONDELETE -> DROP Table when its the last column - count columns in table
//ONDELETE -> DROP database when its the last Table - count tables in database
char* strTooChar(String sQuery)
{
    int sLength = sQuery.length() + 1;                                                                                                      //The string input length.
    char charQuery[sLength];                                                                                                                //A new char array with the length of inputString.
    sQuery.toCharArray(charQuery, sLength); 
}

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

bool bDebugInput = true;
bool bDebugOutput = true;

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

String * MysqlQuery(String sStatement, String sDB, String sTable = "", String sColumn = "", String sInOutValue = "")
{  
           String sQuery = "";                                                                                                              //String variable for our query.
    String * pInOutValue = &sInOutValue;                                                                                                    //A pointer for passing and return values.
        bool bIterateRow = true;
        bool bIterateCol = true;

         bool bCheck_sDB = false;
      bool bCheck_sTable = false;
     bool bCheck_sColumn = false;
    bool bCheck_sInValue = false;

    bool bDb_Get        = false;     
    bool bDb_Create     = false;     
    bool bDb_Delete     = false;     

    bool bTable_Get     = false; 
    bool bTable_Create  = false; 
    bool bTable_Delete  = false; 

    bool bColumn_Get    = false;
    bool bColumn_Create = false;
    bool bColumn_Delete = false;

         if( sStatement == "GET_DATABASE"                       ){ bDb_Get        = true; bCheck_sDB     =  true; bIterateCol = false; bIterateRow =  true; sQuery = MysqlQueryGetDatabase(sDB);                                    } //Get database(sDB) and returns databaseName.
    else if( sStatement == "GET_TABLE"                          ){ bTable_Get     = true; bCheck_sTable  =  true; bIterateCol = false; bIterateRow =  true; sQuery = MysqlQueryGetTable(sDB,sTable);                                } //Get table(sTable) , in database(sDB) and returns tableName.
    else if( sStatement == "GET_COLUMN"                         ){ bColumn_Get    = true; bCheck_sColumn =  true; bIterateCol = false; bIterateRow =  true; sQuery = MysqlQueryGetColumn(sDB,sTable,sColumn);                       } //Get column(sColumn) and returns columnName.
    else if( sStatement == "CREATE_DATABASE"                    ){ bDb_Create     = true; bCheck_sDB     =  true; bIterateCol = false; bIterateRow = false; sQuery = MysqlQueryCreateDatabase(sDB);                                 } //Create a new database(sDB) and returns databaseName.
    else if( sStatement == "CREATE_TABLE"                       ){ bTable_Create  = true; bCheck_sTable  =  true; bIterateCol = false; bIterateRow =  true; sQuery = MysqlQueryCreateTable(sDB,sTable);                             } //Create a new table(sTable), in database(sDB) and returns tableName.    
    else if( sStatement == "CREATE_COLUMN"                      ){ bColumn_Create = true; bCheck_sColumn =  true; bIterateCol = false; bIterateRow =  true; sQuery = MysqlQueryCreateColumn(sDB,sTable,sColumn);                    } //Create a new column(sColumn), in database(sDB), on table(sTable) and returns columnName.
    else if( sStatement == "DELETE_DATABASE"                    ){ bDb_Delete     = true; bCheck_sDB     =  true; bIterateCol = false; bIterateRow =  true; sQuery = MysqlQueryDeleteDatabase(sDB);                                 } //Delete database(sDB) and returns databaseName.
    else if( sStatement == "DELETE_TABLE"                       ){ bTable_Delete  = true; bCheck_sTable  =  true; bIterateCol = false; bIterateRow =  true; sQuery = MysqlQueryDeleteTable(sDB,sTable);                             } //Delete table(sTable), in database(sDB) and returns tableName.
    else if( sStatement == "DELETE_COLUMN"                      ){ bColumn_Delete = true; bCheck_sColumn =  true; bIterateCol = false; bIterateRow =  true; sQuery = MysqlQueryDeleteColumn(sDB,sTable,sColumn);                    } //Delete column(sColumn), in database(sDB), on table(sTable) and returns columnName.

    else if(sStatement == "GET_COLUMN_VALUE_FIRST_SHOW_ONE"     ){ bColumn_Get    = true;                       ; bIterateCol =  true; bIterateRow =  true; sQuery = MysqlQueryGetColumnValueFirstShowOne(sDB, sTable, sColumn);    } //Gets the first value from column(sColumn) on table(sTable) in database(sDB), and returns one value.
    else if(sStatement == "GET_COLUMN_VALUE_LAST_SHOW_ONE"      ){ bColumn_Get    = true;                       ; bIterateCol =  true; bIterateRow =  true; sQuery = MysqlQueryGetColumnValueLastShowOne(sDB, sTable, sColumn);     } //Gets the  last value from column(sColumn) on table(sTable) in database(sDB), and returns one value.
    else if(sStatement == "GET_COLUMN_VALUE_FIRST_SHOW_ROW"     ){ bColumn_Get    = true;                       ; bIterateCol =  true; bIterateRow =  true; sQuery = MysqlQueryGetColumnValueFirstShowRow(sDB, sTable, sColumn);    } //Gets the first value from column(sColumn) on table(sTable) in database(sDB), and returns the whole row.
    else if(sStatement == "GET_COLUMN_VALUE_LAST_SHOW_ROW"      ){ bColumn_Get    = true;                       ; bIterateCol =  true; bIterateRow =  true; sQuery = MysqlQueryGetColumnValueLastShowRow(sDB, sTable, sColumn);     } //Gets the  last value from column(sColumn) on table(sTable) in database(sDB), and returns the whole row.
    
    else if(sStatement == "INSERT_VALUE"                        ){                      ; bCheck_sInValue = false;bIterateCol = false; bIterateRow = false; sQuery = MysqlQueryInsertValue(sDB, sTable, sColumn, sInOutValue);      } //Insert value(sInOutValue) to column(sColumn) on table(sTable) in database(sDB), and returns.
    else if(sStatement == "UPDATE_VALUE"                        ){                      ; bCheck_sInValue = false;bIterateCol = false; bIterateRow = false; sQuery = MysqlQueryUpdateValue(sDB, sTable, sColumn, sInOutValue);      } //Update value(sInOutValue) on column(sColumn) on table(sTable) in database(sDB), and returns.      

    bool bGet    = ( bDb_Get     || bTable_Get     || bColumn_Get    );
    bool bCreate = ( bDb_Create  || bTable_Create  || bColumn_Create );
    bool bDelete = ( bDb_Delete  || bTable_Delete  || bColumn_Delete );
    bool bDb     = ( bDb_Get     || bDb_Create     || bDb_Delete     );
    bool bTable  = ( bTable_Get  || bTable_Create  || bTable_Delete  );
    bool bColumn = ( bColumn_Get || bColumn_Create || bColumn_Delete );

    while(!WifiStatus())WifiConnect();
    //MysqlClientConnect();
    //while (!MysqlClientStatus()){ MysqlClientConnect(); delay(1); };                                                                       //Wait for the connection.
    while (!MysqlClientStatus()){ MysqlClientConnect();};                                                                                   //Wait for the connection.
    if(bDebugInput){
        Serial.println();
        Serial.print("" + StepReturn() + "└─► " + sStatement);
        Serial.print(" ◄─ INPUT ◄── "  + sDB);
        Serial.print(String("") != sTable?" ◄── " + sTable:"");
        Serial.print(String("") != sColumn?" ◄── " + sColumn:"");
        Serial.println(String("") != sInOutValue?" ◄── " + sInOutValue:"");
    }  
   
    MySQL_Cursor *cur_mem  = MysqlQueryCursor(sQuery);//End Query 1                                                                         //Initiate the query class instance, and run the first query

    //Serial.print("\n" + sQuery);

    //To skip iteration for some items
    if(bIterateCol||bIterateRow)
    {           
        //Serial.print(" -> iterate ");                                                                                            
        column_names *cols = cur_mem->get_columns();                                                                                            //Fetch the columns.
        if(bIterateCol)
        {
            //Serial.print(" -> col ");
            for (int iCol = 0; iCol < cols->num_fields; iCol++) {                                                                                   //iterate through columns.
                Serial.print(String(iCol) + " = ");                
                Serial.println(cols->fields[iCol]->name);
            };
        };
        row_values *row = NULL;
        if(bIterateRow)
        {
            //Serial.print(" -> row ");
            do {                                                                                                                                    //Read the rows and print them   
                row = cur_mem->get_next_row();
                if (row != NULL) {                                                                                                                  //iterate through rows while we got results.
                    for (int iRow = 0; iRow < cols->num_fields; iRow++) {        
                        String sInputString  = String(row->values[iRow]);
                        if ( bCheck_sDB )           { if( sInputString == sDB ){sInOutValue = sDB; break;}}
                        else if ( bCheck_sTable )   { if( sInputString == sTable ){sInOutValue = sTable; break;  }}
                        else if ( bCheck_sColumn )  { if( sInputString == sColumn ){sInOutValue = sColumn; break;}}
                        else if ( bCheck_sInValue ) { if( sInputString == sInOutValue ){sInOutValue = sInputString; break;}}
                        else {
                            Serial.println(String(iRow) + " = " + sInputString);                                                                           //Print row number and content
                        };
                    };
                };
            } while (row != NULL);                                                                                                                  //Read till we have all the rows.
        }
    }
                                                                                                                                          //Disconnect from MySql Server.
    delete cur_mem;                                                                                                                       //Deleting the cursor also frees up memory used.
                                                                                        //Return the value to serial monitor. 
    MysqlClientDisconnect();


    if(bDebugOutput){
        Serial.print(StepReturn(1) + "└─► OUTPUT ─► ");

        if(bDb){
            if(bGet){           
                if(sInOutValue == ""){ Serial.print("database not found"); }
                else { Serial.print("database found"); } }       
            else if(bCreate){   
                if(sInOutValue == ""){ Serial.print("database created"); }
                else { Serial.print("database create DB-ERROR  -> " + sInOutValue); } } 
            else if(bDelete){   
                if(sInOutValue == ""){ Serial.print("database deleted"); }
                else { Serial.print("database deleted ERROR  -> " + sInOutValue); } } }
        else if(bTable){
            if(bGet){           
                if(sInOutValue == ""){ Serial.print("table not found"); }    
                else { Serial.print("table found"); } } 
            else if(bCreate){   
                if(sInOutValue == ""){ Serial.print("table created"); }      
                else { Serial.print("table create ERROR  -> " + sInOutValue); } } 
            else if(bDelete){   
                if(sInOutValue == ""){ Serial.print("table deleted"); }      
                else { Serial.print("table deleted ERROR  -> " + sInOutValue); } } } 
        else if(bColumn){   
            if(bGet){           
                if(sInOutValue == ""){ Serial.print("column not found"); }
                else { Serial.print("column found"); } } 
            else if(bCreate){   
                if(sInOutValue == ""){ Serial.print("column created"); }
                else { Serial.print("column create ERROR  -> " + sInOutValue); } } 
            else if(bDelete){   
                if(sInOutValue == ""){ Serial.print("column deleted"); }
                else { Serial.print("column deleted ERROR  -> " + sInOutValue); } }
        };
        iQueryStep++;
    }
    return pInOutValue;                                                                                                                   //Return the value as pointer.
};

void MysqlQueryPreChecker(String sStatement, String sDB, String sTable = "", String sColumn = "", String sInOutValue = ""){

    
    iQueryStep = 1;

    Serial.print("\nRequest: " + setStringLength(sStatement, " ", 31));

    String * pInOutValue = &sInOutValue;   
    bool bDb_Get        = false;     
    bool bDb_Create     = false;     
    bool bDb_Delete     = false;     

    bool bTable_Get     = false; 
    bool bTable_Create  = false; 
    bool bTable_Delete  = false; 

    bool bColumn_Get    = false;
    bool bColumn_Create = false;
    bool bColumn_Delete = false;

    bool bItem_Exists = false;

    String sGetResult;
    String sQueryItem = "";
    String sQueryAction = "";

         if( sStatement == "GET_DATABASE"    ){ bDb_Get        = true; }
    else if( sStatement == "GET_TABLE"       ){ bTable_Get     = true; }
    else if( sStatement == "GET_COLUMN"      ){ bColumn_Get    = true; }  
    else if( sStatement == "CREATE_DATABASE" ){ bDb_Create     = true; }
    else if( sStatement == "CREATE_TABLE"    ){ bTable_Create  = true; }    
    else if( sStatement == "CREATE_COLUMN"   ){ bColumn_Create = true; }
    else if( sStatement == "DELETE_DATABASE" ){ bDb_Delete     = true; }
    else if( sStatement == "DELETE_TABLE"    ){ bTable_Delete  = true; }
    else if( sStatement == "DELETE_COLUMN"   ){ bColumn_Delete = true; }

    bool bGet    = ( bDb_Get     || bTable_Get     || bColumn_Get    );
    bool bCreate = ( bDb_Create  || bTable_Create  || bColumn_Create );
    bool bDelete = ( bDb_Delete  || bTable_Delete  || bColumn_Delete );
    bool bDb     = ( bDb_Get     || bDb_Create     || bDb_Delete     );
    bool bTable  = ( bTable_Get  || bTable_Create  || bTable_Delete  );
    bool bColumn = ( bColumn_Get || bColumn_Create || bColumn_Delete );

         if(bDb)        { sQueryItem = "DATABASE"; sGetResult = *MysqlQuery("GET_DATABASE", sDB);                bItem_Exists = (sGetResult != NULL); }
    else if(bTable)     { sQueryItem = "TABLE";    sGetResult = *MysqlQuery("GET_TABLE", sDB, sTable);           bItem_Exists = (sGetResult != NULL); }
    else if(bColumn)    { sQueryItem = "COLUMN";   sGetResult = *MysqlQuery("GET_COLUMN", sDB, sTable, sColumn); bItem_Exists = (sGetResult != NULL); };


    //OUTPPUT
    //Serial.print("   └─► OUTPUT ◄─► "  + sGetResult);

    if(bGet)
    {
        sQueryAction = "GET";
        //if (!bItem_Exists){
        //    Serial.print("\n\t\t\t └──► Not found..." + sQueryItem + " ─► " + sGetResult);
        //} else {
        //    //Serial.print("\n\t\t\t └──► Found..." + sQueryItem + " ─► " + sGetResult);
        //}         
    }
    else if(bCreate)
    {
        sQueryAction = "CREATE";

        if(!bItem_Exists)//Create request, item does not exist yet.
        {
            //DATABASE - Check databse on, database table and column requests.
            //if( sStatement == "CREATE_DATABASE" && sGetResult == "" || *MysqlQuery("GET_DATABASE" , sDB) == "")//Check if database exists, and dont double check database exists.
            
            
            if(sQueryItem == "DATABASE"){//Skip double gets 
                //database doesnt exist, create Immediately
                MysqlQuery(sQueryAction + "_DATABASE", sDB);
            } 
            else if( *MysqlQuery("GET_DATABASE" , sDB) == "")//Check if database exists, and dont double check database exists.
            {
                MysqlQuery(sQueryAction + "_DATABASE", sDB);//not found, create database.
            };
            //Database must be ok here.

            //TABLE - Check table, on table and column requests.
            if ( sQueryItem != "DATABASE")
            {
                //Serial.println("\n Debug: " + sGetResult );
                if( sStatement == "CREATE_TABLE" && sGetResult == "" || *MysqlQuery("GET_TABLE" , sDB, sTable) == "")//Check if table exists, and dont double check database exists.
                {
                    MysqlQuery(sQueryAction + "_TABLE", sDB, sTable);//not found, create table.
                }

            };
            //Table must be ok here.

            //COLUMN - Check column, on column requests.
            if (sQueryItem == "COLUMN" )
            {
                if(*MysqlQuery("GET_COLUMN" , sDB, sTable, sColumn) == "")//Check if column exists.
                {
                    MysqlQuery(sQueryAction + "_" + sQueryItem, sDB, sTable, sColumn, sInOutValue);//Not found, create column.
                }
            };
            //Column must be ok here.
            
        } else {

            //Serial.print(" -> Found " + sQueryItem + " " + sGetResult + " -> Skip Create...");
            if(bDebugOutput)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Skip Create...");
        }
    } 
    else if(bDelete)
    {
        sQueryAction = "DELETE";

        if(bItem_Exists){
            
            //Serial.print(" -> Found " + sQueryItem + " " + sGetResult);
            MysqlQuery(sQueryAction + "_" + sQueryItem, sDB, sTable , sColumn ,sInOutValue);

        } else {
            //Serial.print(" -> Not Found " + sQueryItem  + " -> Skip delete...");
            if(bDebugOutput)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Skip delete...");
        }
    }
}
void loop()
{

    int r = rand() % 10;


    switch(r){

        case 1: MysqlQueryPreChecker("CREATE_DATABASE", "DatabaseName6"); break;
        case 2: MysqlQueryPreChecker("CREATE_TABLE", "DatabaseName6", "table5"); break;
        case 3: MysqlQueryPreChecker("CREATE_COLUMN", "DatabaseName6", "table5", "column5"); break;
        case 4: MysqlQueryPreChecker("GET_DATABASE", "DatabaseName6"); break;
        case 5: MysqlQueryPreChecker("GET_TABLE", "DatabaseName6", "table5"); break;
        case 6: MysqlQueryPreChecker("GET_COLUMN", "DatabaseName6", "table5", "column5"); break;
        case 7: MysqlQueryPreChecker("DELETE_COLUMN", "DatabaseName6", "table5", "sColumn5"); break;
        case 8: MysqlQueryPreChecker("DELETE_TABLE", "DatabaseName6", "table5"); ; break;
        case 9: MysqlQueryPreChecker("DELETE_DATABASE", "DatabaseName6"); break;
        //case 10: //MysqlQueryPreChecker("INSERT_VALUE", "DatabaseName6", "table5", sColumn, sInOutValue); break;
        //case 11: //MysqlQueryPreChecker("UPDATE_VALUE", "DatabaseName6", "table5", sColumn, sInOutValue); break;
        //case 12: //MysqlQueryPreChecker("GET_COLUMN_VALUE_FIRST_SHOW_ONE", "DatabaseName6", "table5", "sColumn5"); break;
        //case 13: //MysqlQueryPreChecker("GET_COLUMN_VALUE_LAST_SHOW_ONE", "DatabaseName6", "table5", "sColumn5"); break;
        //case 14: //MysqlQueryPreChecker("GET_COLUMN_VALUE_FIRST_SHOW_ROW", "DatabaseName6", "table5", "sColumn5"); break;
        //case 15: //MysqlQueryPreChecker("GET_COLUMN_VALUE_LAST_SHOW_ROW", "DatabaseName6", "table5", "sColumn5"); break;
    }           

    //Serial.print("\n.............................................................. loop:" + String(loopCounter++) + " ..............................................................");   
    //delay(1000); //Slow down between loops
};
void setup() {
    Serial.begin(115200);   //Start serial monitor.
    while (!Serial);        //Wait for serial port to connect.
    Serial.println();       //Start the console on a new line.
    WifiConnect();          //Connect the WifiClient.
};






