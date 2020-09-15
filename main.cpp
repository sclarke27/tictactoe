#include <iostream>
using namespace std;

// variables to handle game board
char digits[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
char turnHistory[9];
char matrix[9];
// variables for UI
string white = "\x1B[37m";
string red = "\x1B[91m";
string green = "\x1B[92m";
string player1Color = "\x1B[95m";
string player2Color = "\x1B[96m";
string colDelimiter = white + " | ";
string leftPadding = "     ";
string dLeftPadding = leftPadding + leftPadding + "  ";
string error = "";
char p1Char = 'X';
char p2Char = 'O';
// variables for game state
int p1Wins = 0;
int p2Wins = 0;
int currentPlayer = 1;
int turnCount = 0;
int winner = 0;

/*
	Pick what color a game board field should be based on its content
*/
string FormatColor(char field) {
	string returnStr = white + field;
	if (field == p1Char) {
		returnStr = player1Color + p1Char;
	}
	if (field == p2Char) {
		returnStr = player2Color + p2Char;
	}
	return returnStr;
}

/*
	Clear the console and redraw the game UI based on current game state (title, players, score, game board)
*/
void DrawUI() {
	system("CLS");
	cout << endl;
	cout << green << " ---==== Simple Tic-Tac-Toe ====--- " << endl;
	cout << dLeftPadding << red << error << white << endl;
	cout << leftPadding << player1Color << "Player #1: " << p1Char << colDelimiter << player2Color << "Player #2: " << p2Char << endl;
	if (p1Wins > 0 || p2Wins > 0) {
		cout << leftPadding << player1Color << "     Wins: " << p1Wins << colDelimiter << player2Color << "Wins: " << p2Wins << endl;
	}	
	cout << endl;
	cout << dLeftPadding << " " << FormatColor(matrix[0]) << colDelimiter << FormatColor(matrix[1]) << colDelimiter << FormatColor(matrix[2]) << endl;
	cout << dLeftPadding << white << "---+---+---\n";
	cout << dLeftPadding << " " << FormatColor(matrix[3]) << colDelimiter << FormatColor(matrix[4]) << colDelimiter << FormatColor(matrix[5]) << endl;
	cout << dLeftPadding << white << "---+---+---\n";
	cout << dLeftPadding << " " << FormatColor(matrix[6]) << colDelimiter << FormatColor(matrix[7]) << colDelimiter << FormatColor(matrix[8]) << endl;
	cout << white << endl;
	cout << dLeftPadding << "   Turn: " << (turnCount + 1) << endl;
	cout << white << endl;
	error = "";
}

/*
	Undo previous move.
*/
void UndoLastMove() {
	if (turnCount > 0) {
		char lastField = turnHistory[turnCount];
		matrix[lastField] = digits[lastField];
		turnCount--;
		currentPlayer = (currentPlayer == 1) ? 2 : 1;
	}
}

/*
	utility method to reset the game board data
*/
void ResetMatrix() {
	for (int i = 0; i < 9; i++) {
		matrix[i] = digits[i];
	}
}

/*
	Wait for user input during gameplay
*/
int GetUserInput() {

	cout << leftPadding << "Select a field " << ((currentPlayer == 1) ? player1Color : player2Color) << "Player #" << currentPlayer << white << ":";
	char inputValue;
	int returnValue = -1;
	cin >> inputValue;

	// check for undo
	if (inputValue == 'u') {
		cout << "UNDO!" << endl;
		UndoLastMove();
	}
	else {
		if (isdigit(inputValue)) {			
			returnValue = inputValue - '0';
		}
	}

	return returnValue;
}

/*
	Check all possible win states for a win
*/
void CheckForWin() {
	// check for wins from slot zero
	// across
	if (winner == 0 && matrix[0] == matrix[1] && matrix[1] == matrix[2]) {
		winner = currentPlayer;
	}
	// diagnal
	if (winner == 0 && matrix[0] == matrix[4] && matrix[4] == matrix[8]) {
		winner = currentPlayer;
	}
	// down
	if (winner == 0 && matrix[0] == matrix[3] && matrix[3] == matrix[6]) {
		winner = currentPlayer;
	}

	//check for win from slot 1
	// down
	if (winner == 0 && matrix[1] == matrix[4] && matrix[4] == matrix[7]) {
		winner = currentPlayer;
	}

	//check for win from slot 2
	// diagnal
	if (winner == 0 && matrix[2] == matrix[4] && matrix[4] == matrix[6]) {
		winner = currentPlayer;
	}
	// down
	if (winner == 0 && matrix[2] == matrix[5] && matrix[5] == matrix[8]) {
		winner = currentPlayer;
	}

	// check for wins from slot 3
	// across
	if (winner == 0 && matrix[3] == matrix[4] && matrix[4] == matrix[5]) {
		winner = currentPlayer;
	}

	// check for wins from slot 6
	// across
	if (winner == 0 && matrix[6] == matrix[7] && matrix[7] == matrix[8]) {
		winner = currentPlayer;
	}

}

/*
	Update a specific field on the board
*/
void UpdateField(int fieldValue) {
	if (fieldValue >= 1 && fieldValue <= 9) {
		int selectedField = fieldValue - 1;
		if (matrix[selectedField] != p1Char && matrix[selectedField] != p2Char) {
			matrix[selectedField] = (currentPlayer == 1) ? p1Char : p2Char;
			CheckForWin();
			currentPlayer = (currentPlayer == 1) ? 2 : 1;
			turnCount++;
			turnHistory[turnCount] = selectedField;
		}
		else {
			// set error to show user
			error = "Field taken";
		}
	}
	else {
		if (fieldValue != -1) {
			// set error to show user
			error = "Enter 1-9.";
		}
	}

}

/*
	Core game loop
*/
void GameLoop() {
	//startup
	ResetMatrix(); // clear game board
	DrawUI(); 

	// game loop
	while (turnCount < 9 && winner == 0) {
		UpdateField(GetUserInput());	// wait for user input and update game state once input given
		DrawUI();	// redraw ui for new state
	}

	// if we have a winner, append win message to UI
	if (winner != 0) {
		string playerColor;
		if (winner == 1) {
			playerColor = player1Color;
			p1Wins++;
		}
		else {
			playerColor = player2Color;
			p2Wins++;
		}
		cout << leftPadding << " **** " << playerColor << "Player " << winner << " won!!" << white << " **** " << endl;

	}

	// if there is no winner, append draw message to UI
	if (winner == 0) {
		cout << white << leftPadding << "It was a draw." << endl;
	}

	// since we got past the game loop, ask to play again.
	char newInput;
	cout << endl;
	cout << leftPadding << "Play Again? (y/n) ";
	cin >> newInput;

	// if user hit 'y' reset game and restart game loop
	if (newInput == 'y') {
		ResetMatrix();
		turnCount = 0;
		winner = 0;
		GameLoop();
	}
	else {
		// make sure we dont leave random colors enabled on exit
		cout << endl << leftPadding << green << "Thanks for playing!!" << white << endl << endl;
	}

}

/*
	app entry point
*/
int main() {
	// start game
	GameLoop();

	return 0;
}