function saveSettings() {

    var db = LocalStorage.openDatabaseSync("KoronakoDB", "1.0", "Koronako database", 1000000);

    db.transaction(
                function(tx) {
                    // Create the table, if not existing
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Settings(name TEXT, subname TEXT, valte TEXT, valre REAL, valint INTEGER)');

                    // covidStartDate
                    var rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'covidStartDate');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valte=? WHERE name=?', [covidStartDate, 'covidStartDate'])}
                    else if (covidStartDate != ""){tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'covidStartDate', '', covidStartDate, '', '' ])}
                    // covidEndDate
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'covidEndDate');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valte=? WHERE name=?', [covidEndDate, 'covidEndDate'])}
                    else if (covidEndDate != ""){tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'covidEndDate', '', covidEndDate, '', '' ])}
                    // minHits
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'minHits');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valint=? WHERE name=?', [ minHits, 'minHits'])}
                    else {tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'minHits', '', '', '' , minHits])}
                    // discoveryTimer
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'discoveryTimer');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valint=? WHERE name=?', [ discoveryTimer, 'discoveryTimer'])}
                    else {tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'discoveryTimer', '', '', '' , discoveryTimer])}
                    // serverAddress
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'serverAddress');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valte=? WHERE name=?', [serverAddress, 'serverAddress'])}
                    else {tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'serverAddress', '', serverAddress, '', '' ])}
                    // serverPort
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'serverPort');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valint=? WHERE name=?', [ serverPort, 'serverPort'])}
                    else {tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'serverPort', '', '', '' , serverPort])}
                    // coronaExposureSince
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'coronaExposureSince');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valint=? WHERE name=?', [ coronaExposureSince, 'coronaExposureSince'])}
                    else {tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'coronaExposureSince', '', '', '' , coronaExposureSince])}
                    //VISIBILITY settings
                    // visi_closeText
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'visi_closeText');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valint=? WHERE name=?', [ closeText.visible, 'visi_closeText'])}
                    else {tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'visi_closeText', '', '', '' , closeText.visible])}
                    // visi_exposuresText
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'visi_exposuresText');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valint=? WHERE name=?', [ exposuresText.visible, 'visi_exposuresText'])}
                    else {tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'visi_exposuresText', '', '', '' , exposuresText.visible])}
                    // visi_koronaExposuresText
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'visi_koronaExposuresText');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valint=? WHERE name=?', [ koronaExposuresText.visible, 'visi_koronaExposuresText'])}
                    else {tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'visi_koronaExposuresText', '', '', '' , koronaExposuresText.visible])}
                    // visi_koronaDiseaseText
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', 'visi_koronaDiseaseText');
                    if (rs.rows.length > 0) {tx.executeSql('UPDATE Settings SET valint=? WHERE name=?', [ koronaDiseaseText.visible, 'visi_koronaDiseaseText'])}
                    else {tx.executeSql('INSERT INTO Settings VALUES(?, ?, ?, ?, ?)', [ 'visi_koronaDiseaseText', '', '', '' , koronaDiseaseText.visible])}
                }
                )
}

function loadSettings() {

    var db = LocalStorage.openDatabaseSync("KoronakoDB", "1.0", "Koronako database", 1000000);

    db.transaction(
                function(tx) {
                    // Create the table, if not existing
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Settings(name TEXT, subname TEXT, valte TEXT, valre REAL, valint INTEGER)');

                    // covidStartDate
                    var rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['covidStartDate']);
                    if (rs.rows.length > 0) {covidStartDate = rs.rows.item(0).valte}
                    else {}
                    // covidEndDate
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['covidEndDate']);
                    if (rs.rows.length > 0) {covidEndDate = rs.rows.item(0).valte}
                    else {}
                    // minHits
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['minHits']);
                    if (rs.rows.length > 0) {minHits = rs.rows.item(0).valint}
                    else {}
                    // discoveryTimer
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['discoveryTimer']);
                    if (rs.rows.length > 0) {discoveryTimer = rs.rows.item(0).valint}
                    else {}
                    // serverAddress
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['serverAddress']);
                    if (rs.rows.length > 0) {serverAddress = rs.rows.item(0).valte}
                    else {}
                    // minHits
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['minHits']);
                    if (rs.rows.length > 0) {minHits = rs.rows.item(0).valint}
                    else {}
                    // serverPort
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['serverPort']);
                    if (rs.rows.length > 0) {serverPort = rs.rows.item(0).valint}
                    else {}
                    // coronaExposureSince
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['coronaExposureSince']);
                    if (rs.rows.length > 0) {coronaExposureSince = rs.rows.item(0).valint}
                    else {}
                    //VISIBILITY settings
                    // closeText.visible
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['visi_closeText']);
                    if (rs.rows.length > 0) {closeText.visible = rs.rows.item(0).valint}
                    else {}
                    // visi_exposuresText
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['visi_exposuresText']);
                    if (rs.rows.length > 0) {exposuresText.visible = rs.rows.item(0).valint}
                    else {}
                    // visi_koronaExposuresText
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['visi_koronaExposuresText']);
                    if (rs.rows.length > 0) {koronaExposuresText.visible = rs.rows.item(0).valint}
                    else {}
                    // visi_koronaDiseaseText
                    rs = tx.executeSql('SELECT * FROM Settings WHERE name = ?', ['visi_koronaDiseaseText']);
                    if (rs.rows.length > 0) {koronaDiseaseText.visible = rs.rows.item(0).valint}
                    else {}
                }
                )
}

// Function adds hits to devices when devices scanned
function addHits(_devicepair, _day) {

    var db = LocalStorage.openDatabaseSync("KoronakoDB", "1.0", "Koronako database", 1000000);

    db.transaction(
                function(tx) {
                    // Create the table, if not existing
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Exposures(devicepair TEXT, day TEXT, hits INTEGER)');

                    // Check if the device is already in the database
                    var rs = tx.executeSql('SELECT * FROM Exposures WHERE devicepair = ? AND day = ?', [_devicepair, _day]);
                    // If multiple devices during day, something went wrong
                    if (rs.rows.length > 1){console.log("error 1")}
                    // Increase hits by 1
                    else if (rs.rows.length > 0) {
                        tx.executeSql('UPDATE Exposures SET hits=? WHERE devicepair = ? AND day = ?', [rs.rows.item(0).hits + 1, _devicepair, _day])}
                    // If no hits during day, add a new hit
                    else {tx.executeSql('INSERT INTO Exposures VALUES(?, ?, ?)', [_devicepair,_day, 1])}
                }
                )
}

// Function finds devices od the selected day
function findHits(_day) {

    var db = LocalStorage.openDatabaseSync("KoronakoDB", "1.0", "Koronako database", 1000000);

    db.transaction(
                function(tx) {
                    // Create the table, if not existing
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Exposures(devicepair TEXT, day TEXT, hits INTEGER)');

                    // All devices
                    // Check count of the devices in db
                    var rs = tx.executeSql('SELECT count(devicepair) AS cdevicepair FROM Exposures WHERE day = ?', [_day]);
                    // If multiple results, something went wrong
                    if (rs.rows.length > 1){console.log("error 1")}
                    // Set new result to the list model
                    else if (rs.rows.length > 0) {
                        koronaList.set(0,{"devices": rs.rows.item(0).cdevicepair})
                    }
                    // If no search results do nothing
                    else {}

                    // Devices with hiths more than requested
                    // Check devices with more than minHits hits
                    rs = tx.executeSql('SELECT count(devicepair) AS cdevicepair FROM Exposures WHERE day = ? AND hits > ?', [_day, minHits]);
                    // If multiple results, something went wrong
                    if (rs.rows.length > 1){console.log("error 1")}
                    // Set new result to the list model
                    else if (rs.rows.length > 0) {
                        koronaList.set(0,{"exposures": rs.rows.item(0).cdevicepair})
                        //console.log("exposures",rs.rows.item(0).cdevicepair)
                    }
                    // If no search results do nothing
                    else {}
                }
                )
}

// Function deletes old data from db
function deleteOldData(_day) {

    var db = LocalStorage.openDatabaseSync("KoronakoDB", "1.0", "Koronako database", 1000000);

    db.transaction(
                function(tx) {
                    //https://stackoverflow.com/questions/2998784/how-to-output-numbers-with-leading-zeros-in-javascript
                    function pad(a,b){return(1e15+a+"").slice(-b)}
                    // Create the table, if not existing
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Exposures(devicepair TEXT, day TEXT, hits INTEGER)');
                    // Deleting phones below hit limit from previous days
                    tx.executeSql('DELETE FROM Exposures WHERE substr(devicepair,1,2) != ? AND hits < ?', [_day, minHits + 1]);
                    var tday = Number(_day)
                    var textday = ''
                    for (var i = 1; i<32;i++){

                        // Deleting old data from database
                        if (i < (tday-25)){
                            tx.executeSql('DELETE FROM Exposures WHERE substr(devicepair,1,2) = ?', [pad(i,2)]);
                        }
                        else if (i>tday && i < (tday +5)){
                            tx.executeSql('DELETE FROM Exposures WHERE substr(devicepair,1,2) = ?', [pad(i,2)]);
                        }
                    }
                }
                )
}

function checkMyExposures() {

    var db = LocalStorage.openDatabaseSync("KoronakoDB", "1.0", "Koronako database", 1000000);

    db.transaction(
                function(tx) {
                    // Create the table, if not existing
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Exposures(devicepair TEXT, day TEXT, hits INTEGER)');

                    var rs = tx.executeSql('SELECT devicepair FROM Exposures WHERE hits > ?', [minHits]);
                    var _exposurelist = ''
                    for (var i = 1; i<rs.rows.length;i++){

                        _exposurelist = _exposurelist +  rs.rows.item(i).devicepair
                    }
                    koronaClient.expdata = _exposurelist;
                    return "KoronaTest"
                }
                )
}

function readMyKorona() {

    var db = LocalStorage.openDatabaseSync("KoronakoDB", "1.0", "Koronako database", 1000000);

    db.transaction(
                function(tx) {
                    // Create the table, if not existing
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Exposures(devicepair TEXT, day TEXT, hits INTEGER)');
                    // Because having only day numbers month has to be devided to two part
                    var ls  //Lower start day -1
                    var le // Lower end day + 1
                    var hs
                    var he
                    console.log(((new Date()-new Date(covidEndDate))/24/3600/1000))
                    if (((new Date()-new Date(covidEndDate))/24/3600/1000)>25) {
                        console.log("No data to be sent")
                    }
                    else if (((new Date()-new Date(covidStartDate))/24/3600/1000)>25){
                        if (new Date().getDate()>25){
                            ls = hs = new Date().getDate()-25
                            le = new Date(covidEndDate).getDate()
                            console.log(">25")
                        }
                        else {
                            ls = 31 + new Date().getDate()-25
                            le = 31
                            hs = 1
                            console.log("<25")
                        }

                        he = new Date(covidEndDate).getDate()
                        console.log("Only enddata affecting", ls, le, hs, he)
                    }
                    else if(new Date(covidStartDate).getMonth() != new Date(covidEndDate).getMonth()){
                        ls = new Date(covidStartDate).getDate()
                        le = 31
                        hs = 1
                        he = new Date(covidEndDate).getDate()
                        console.log("Different month", ls, le, hs, he)
                    }
                    else {
                        ls = hs = new Date(covidStartDate).getDate()
                        le = he = new Date(covidEndDate).getDate()
                        console.log("Clear case", ls, le, hs, he)
                    }


                    var rs = tx.executeSql('SELECT devicepair FROM Exposures WHERE hits > ? AND substr(devicepair,3,1) = ?', [minHits, ':']);
                    var _koronalist = '00:00:00:00:00:00'
                    var moved = ''
                    for (var i = 1; i<rs.rows.length;i++){
                        var _poll = Number(rs.rows.item(i).devicepair.substring(0,2))
                        if (_poll >= ls && _poll <= le || _poll >= hs && _poll <= he){
                            _koronalist = _koronalist +  rs.rows.item(i).devicepair
                            //This logic has to be revised
                            //moved = rs.rows.item(i).devicepair.substring(0,2) + '.' + rs.rows.item(i).devicepair.substring(3,17)
                            //console.log(moved)
                            //tx.executeSql('UPDATE Exposures SET devicepair=? WHERE devicepair=?', [ moved, rs.rows.item(i).devicepair])
                        }
                    }

                    //koronaClient.korodata = _koronalist;
                    koronaClient.expdata = _koronalist;
                    console.log(_koronalist)
                    return "MyKoronaData"
                }
                )
}
