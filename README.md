# Quizzer – C++ Quiz Management System

**Quizzer** is a complete quiz management system built in C++ that allows users to create, edit, and take multiple-choice quizzes via a text-based interface. It uses file handling to store quizzes and supports robust input validation to ensure smooth user interaction.

## Features

- Create and save new quizzes
- Input multiple-choice questions with four options and a correct answer
- Store quiz data in structured text files
- View and select from existing quizzes
- Start a quiz and receive score feedback
- Edit quizzes by adding or deleting questions
- Delete entire quizzes
- Input validation to handle incorrect or unexpected user input

## Sample Data

This repository includes a `sample` folder with example data files.

To use the sample data:

1. Copy the `.txt` files from the `sample` folder.
2. Paste them into the `Data/` directory of your project.
3. Replace the existing `Quizzes.txt` file if prompted.

This allows you to explore the program using pre-defined quizzes.

## Requirements

- A C++11 or later compatible compiler (e.g., g++)
- A terminal or console to run the compiled application
- A `Data/` directory in the root of the project to store all quiz files

## Notes

- Ensure the `Data` folder exists before running the program.
- All quizzes and questions are stored in plain text for simplicity and portability.
- Always back up `Quizzes.txt` before making significant changes or replacing with sample data.
