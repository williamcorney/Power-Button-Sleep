# PowerButtonSleep

Monitor power button being pressed and send system to sleep after 5 seconds

## Installation Instructions

1.  Place binary in a folder of your choice.  I place it in my home folder
2.  You may need to run chmod +x to ensure binary is executable.
3.  Run this command to create a cronjob.  Amend the path of course.

    ```(crontab -l 2>/dev/null; echo "@reboot /Users/williamcorney/SleepMonitor") | crontab - ```
4.  You can check the job is created by running crontab -e
