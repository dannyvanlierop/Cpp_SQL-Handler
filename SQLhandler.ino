
#if defined(ARDUINO_ESP32_DEV)
    #include <WiFi.h>
    #include <WiFiClient.h>
#elif defined (ARDUINO_ESP8266_WEMOS_D1MINI)
    #include <ESP8266WiFi.h>
#endif

#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

IPAddress mySQLServerIP = IPAddress(0, 0, 0, 0);
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

bool bDebugInput  = bDebug;
bool bDebugOutput = bDebug;

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
    bIterateRow = ( bIterateRow && !bCreate );          //Iterate rows by default, only skip on create
    bColumn_Get = ( bColumn_Get || bValue_Get );        //Check columns existence before fetching

    while( !WifiStatus() ) WifiConnect();               //Wait for the connection.
    while( !MysqlClientStatus() ) MysqlClientConnect(); //Wait for the connection.

    if(bDebugInput){
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
            if(bDebugOutput)Serial.print(StepReturn() + "\t└┐\n");
            for (int iCol = 0; iCol < cols->num_fields; iCol++) {   //iterate through columns.               
                if(bDebugOutput)Serial.print(StepReturn() + "\t ├─► col" + String(iCol) + " = ");                
                if(bDebugOutput)Serial.println(cols->fields[iCol]->name);
            };
            if(bDebugOutput)Serial.print(StepReturn() + "\t┌┘\n");
        };
        row_values *row = NULL;
        if(bIterateRow)
        {
            do {    //Read the rows and print them   
                row = cur_mem->get_next_row();
                if (row != NULL) {  //iterate through rows while we got results.
                    if(bDebugOutput)Serial.print(StepReturn() + "\t└┐\n");
                    for (int iRow = 0; iRow < cols->num_fields; iRow++) {        
                        String sInputString  = String(row->values[iRow]);
                        if(bDebugOutput)Serial.println(StepReturn() + "\t ├─► row" + String(iRow) + " = " + sInputString);  //Print row number and content

                        if ( bDb ) {                        if( sInputString == sDB ){        sInOutValue = sDB;       break; } }                          
                        else if ( bTable ) {                if( sInputString == sTable ){     sInOutValue = sTable;    break; } }
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
            if(bDebugOutput)Serial.print(StepReturn() + "\t┌┘\n");
        }
    }
    
    delete cur_mem;//Deleting the cursor also frees up memory used.

    MysqlClientDisconnect();//Disconnect from MySql Server.

    if(bDebugOutput){
        Serial.print(StepReturn(1) + "└─► OUTPUT ─► ");

        if (bValue){
                if(sInOutValue == ""){ Serial.print("bValue not found"); }
                else { Serial.print("bValue " + sInOutValue + " found"); } 
        } 
        else if(bInsert){
                if(sInOutValue == ""){ Serial.print("bInsert not found"); }
                else { Serial.print(sInOutValue); } 
        } 
        else if(bUpdate){
                if(sInOutValue == ""){ Serial.print("bUpdate not found"); }
                else { Serial.print(sInOutValue); } 
        } 
        else if(bDb){
            if(bGet){           
                if(sInOutValue == ""){ Serial.print("database not found"); }
                else { Serial.print("database " + sInOutValue + " found"); } }       
            else if(bCreate){   
                if(sInOutValue == ""){ Serial.print("database created"); }
                else { Serial.print("database " + sInOutValue + " create DB-ERROR  -> " + sInOutValue); } } 
            else if(bDelete){   
                if(sInOutValue == ""){ Serial.print("database deleted"); }
                else { Serial.print("database " + sInOutValue + " deleted ERROR  -> " + sInOutValue); } } }
        else if(bTable){
            if(bGet){           
                if(sInOutValue == ""){ Serial.print("table not found"); }    
                else { Serial.print("table " + sInOutValue + " found"); } } 
            else if(bCreate){   
                if(sInOutValue == ""){ Serial.print("table created"); }      
                else { Serial.print("table " + sInOutValue + " create ERROR  -> " + sInOutValue); } } 
            else if(bDelete){   
                if(sInOutValue == ""){ Serial.print("table deleted"); }      
                else { Serial.print("table " + sInOutValue + " deleted ERROR  -> " + sInOutValue); } } } 
        else if(bColumn){ 
            if(bGet){           
                if(sInOutValue == ""){ Serial.print("column not found"); }
                else { Serial.print("column " + sInOutValue + " found"); } } 
            else if(bCreate){   
                if(sInOutValue == ""){ Serial.print("column created"); }
                else { Serial.print("column " + sInOutValue + " create ERROR  -> " + sColumn); } } 
            else if(bDelete){   
                if(sInOutValue == ""){ Serial.print("column deleted"); }
                else { Serial.print("column " + sInOutValue + " deleted ERROR  -> " + sInOutValue); } }
        };
        iQueryStep++;
    }
    return pInOutValue; //Return the value as pointer.
};

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

    if(bDebugInput)Serial.print("\nRequest:" + setStringLength(sStatement, " ", 31));

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
        if(bDebugInput)Serial.print(" ─► Skip...");
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
                    MysqlQueryDo("INSERT_VALUE", sDB, sTable, sColumn, "TMPnull");//Create init value 
                }
            };//Column must be ok here.          
        }     
        else {
            if(bDebugOutput && bCreate)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Skip Create...");
            if(bDebugOutput && bInsert)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Do Insert...");
            if(bDebugOutput && bUpdate)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Do Update...");
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
            if(bDebugOutput)Serial.print("\n" + StepReturn(iQueryStepIncrease) + "└───► Skip delete...");//Skip deleting non existing items
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
void loop()
{
    int r = rand() % 16;

    switch(r){

        case 1:  MysqlQuery("CREATE_DATABASE", "DatabaseName6"); break;
        case 2:  MysqlQuery("CREATE_TABLE", "DatabaseName6", "table5"); break;
        case 3:  MysqlQuery("CREATE_COLUMN", "DatabaseName6", "table5", "column5"); break;
        case 4:  MysqlQuery("GET_DATABASE", "DatabaseName6"); break;
        case 5:  MysqlQuery("GET_TABLE", "DatabaseName6", "table5"); break;
        case 6:  MysqlQuery("GET_COLUMN", "DatabaseName6", "table5", "column5"); break;
        case 7:  MysqlQuery("DELETE_COLUMN", "DatabaseName6", "table5", "sColumn5"); break;
        case 8:  MysqlQuery("DELETE_TABLE", "DatabaseName6", "table5"); ; break;
        case 9:  MysqlQuery("DELETE_DATABASE", "DatabaseName6"); break;
        case 10: MysqlQuery("INSERT_VALUE", "DatabaseName6", "table5", "column5", String(loopCounter)); break;
        case 11: MysqlQuery("UPDATE_VALUE", "DatabaseName6", "table5", "column5", String(loopCounter)); break;
        case 12: MysqlQuery("GET_COLUMN_VALUE_FIRST_SHOW_ONE", "DatabaseName6", "table5", "column5"); break;
        case 13: MysqlQuery("GET_COLUMN_VALUE_LAST_SHOW_ONE", "DatabaseName6", "table5", "column5"); break;
        case 14: MysqlQuery("GET_COLUMN_VALUE_FIRST_SHOW_ROW", "DatabaseName6", "table5", "column5"); break;
        case 15: MysqlQuery("GET_COLUMN_VALUE_LAST_SHOW_ROW", "DatabaseName6", "table5", "column5"); break;

    }           

    Serial.print("\n.............................................................. loop:" + String(loopCounter++) + " ..............................................................");   
    //delay(1000); //Slow down between loops
    iQueryStep = 1;//Reset
};
void setup() {
    Serial.begin(250000);   //Start serial monitor.
    while (!Serial);        //Wait for serial port to connect.
    Serial.println();       //Start the console on a new line.
    WifiConnect();          //Connect the WifiClient.
};






