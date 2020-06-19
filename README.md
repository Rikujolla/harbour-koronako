# harbour-koronako
Covid 19 trial


The whole software is licensed under BSD.

SOFTWARE DEVELOPMENT AND RELEASE PROCESS

There will be master branch which is a code for the releases in Jolla Harbour, OpenRepos.net and Mer OBS. The phases for the release are:

1. The new version will be devoloped in dev(version) branch
2. Translation are updated in most recent dev branch
3. When translations are ready check their visualization
4. Remove extra console.logs from the code
5. Finalize harbour-koronako.changes file
6. Test the app
NOT WORKING YET 7. Do Jolla Harbour tests for the rpms
8. Commit changes for the version, amend commits if changes are needed in the test process
9. Merge dev branch to the master
10. Move the source to GitHub
NOT WORKING YET 11. Create package from GitHub master to the Mer OBS (tar -czvf harbour-koronako-0.0.8.tar.bz2 ...)
NOT WORKING YET 12. Load the local rpms to Jolla Harbour and edit release info
13. Load the local rpms to OpenRepos and edit release info


PRIVACY
If you are thinking privacy aspects of this code you can check the folder as follows from your phone:
/var/lib/bluetooth/XX:XX:XX:XX:XX:XX/cache and compare that data saved in .local/share/harbour-koronako/harbour-koronako/QML/OfflineStorage/Databases/
