# windows-tasks-services-checker

## About

`windows-tasks-services-checker` – allows displaying the status of the Windows services and tasks to the console. In the `config.txt` file you need to specify a list of Windows services and tasks whose status you want to output to the console. In this file also specifies the required state for each services and tasks. The program compares the current state with the required state and displays the result of the comparison to the console.

Example output:
![example output](https://github.com/OlegShabalov/windows-tasks-services-checker/blob/master/Screenshots/Screenshot1.png "example output")

## Usage

The `config.txt` file contains an example of how to fill it out. Fill out the `config.txt` file to suit your needs.

The `config.txt` file must be located in the same directory as the `.exe` file. Make sure this file has been copied to the build directory.

Administrator rights may be required to get the current status of some tasks. Without administrator rights, such tasks would be assigned the ERROR status.

## License

[MIT license](https://github.com/OlegShabalov/windows-tasks-services-checker/blob/master/LICENSE)