
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
                    rs = tx.executeSql('SELECT count(devicepair) AS cdevicepair FROM Exposures WHERE day = ? AND hits > ?', [_day, 7]);
                    // If multiple results, something went wrong
                    if (rs.rows.length > 1){console.log("error 1")}
                    // Set new result to the list model
                    else if (rs.rows.length > 0) {
                        koronaList.set(0,{"exposures": rs.rows.item(0).cdevicepair})
                        //console.log("exposures",rs.rows.item(0).cdevicepair)
                    }
                    // If no search results do nothing
                    else {}

                    devicesSeen.text = qsTr("Devices seen today") + ": " + koronaList.get(0).devices
                    exposuresMet.text = qsTr("Device exposures today") + ": " + koronaList.get(0).exposures
                }
                )

}
