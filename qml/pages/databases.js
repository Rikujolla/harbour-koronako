
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

// Function adds hits to devices when devices scanned
function findHits(_day) {

    var db = LocalStorage.openDatabaseSync("KoronakoDB", "1.0", "Koronako database", 1000000);

    db.transaction(
                function(tx) {
                    // Create the table, if not existing
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Exposures(devicepair TEXT, day TEXT, hits INTEGER)');

                    // Check count of the devices in db
                    var rs = tx.executeSql('SELECT count(devicepair) AS cdevicepair FROM Exposures WHERE day = ?', [_day]);
                    // If multiple results, something went wrong
                    if (rs.rows.length > 1){console.log("error 1")}
                    // Set new result to the list model
                    else if (rs.rows.length > 0) {
                        koronaList.set(0,{"devices": rs.rows.item(0).cdevicepair})
                        //console.log("devices",rs.rows.item(0).cdevicepair)
                    }
                    // If no search results do nothing
                    else {}

                    // Check deviices with more than 7 hits
                    //rs = tx.executeSql('SELECT day, count(devicepair) AS cdevicepair FROM Exposures WHERE day = ? GROUP BY day HAVING cdevicepair > ?', [_day, 7]);
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

                    //devicesSeen.text = qsTr("Devices seen today") + ": " + koronaList.get(0).devices
                    //exposuresMet.text = qsTr("Device exposures today") + ": " + koronaList.get(0).exposures
                    exposuresText.text = qsTr("Korona app has detected today %1 phones close of your phone.").arg(koronaList.get(0).devices)
                            + qsTr(" Of those phones %1 have exceeded determined exposure time.").arg(koronaList.get(0).exposures)
                    koronaExposuresText.text = qsTr("By sending your exposure data to the server, you can check if somebody has exposured you to coronavirus")

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
                    var tday = Number(_day)
                    var textday = ''
                    for (var i = 1; i<32;i++){

                        // Delete
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
                }
                )

}
