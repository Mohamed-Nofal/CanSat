package ploto;
import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import javafx.stage.FileChooser.ExtensionFilter;

import javax.swing.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
public class ReadTextFileFX extends Application implements EventHandler<ActionEvent>
{
  private BufferedReader reader = null;
  private int lineCount = 0;

  private Button buttonSelectFile;
  private Button buttonReadLine;
  private Label labelFileName;
  private Text textLineFields;
  private ChoiceBox choiceBoxDelimiter;

  private static final int WINDOW_WIDTH  = 500;
  private static final int WINDOW_HEIGHT = 300;


  //============================================================================================
  // This method is called once by JavaFX.launch()
  // This method sets the window (Stage primaryStage) title.
  // It also creates and the GUI components.
  //============================================================================================
  @Override
  public void start(Stage primaryStage)
  {
    primaryStage.setTitle("File Reader");
    primaryStage.setResizable(false);

    buttonSelectFile = new Button("Select Input Text File");
    buttonSelectFile.setOnAction(this);


    buttonReadLine = new Button("Read Line");
    buttonReadLine.setOnAction(this);
    buttonReadLine.setDisable(true); //User cannot read line until a file has been opened.

    labelFileName = new Label();
    textLineFields = new Text();

    choiceBoxDelimiter = new ChoiceBox(FXCollections.observableArrayList(
      "Delimiter: Tab", "Delimiter: Comma", "Delimiter: Space")
    );
    choiceBoxDelimiter.getSelectionModel().select(0);

    //Make both buttons the same width.
    buttonSelectFile.setMaxWidth(WINDOW_WIDTH/2);
    labelFileName.setMaxWidth(WINDOW_WIDTH/2);
    choiceBoxDelimiter.setMaxWidth(WINDOW_WIDTH/2);
    buttonReadLine.setMaxWidth(WINDOW_WIDTH/2);
    textLineFields.setWrappingWidth(WINDOW_WIDTH-20);

    VBox buttonBox = new VBox();
    buttonBox.setPadding(new Insets(10, 10, 10, 10));  //Sets the space around the buttonBox.
    buttonBox.setSpacing(10);  //Sets the vertical space in pixels between buttons within the box.

    buttonBox.getChildren().addAll(
      buttonSelectFile,
      labelFileName,
      choiceBoxDelimiter,
      buttonReadLine,
      textLineFields
    );

    StackPane root = new StackPane();
    root.getChildren().add(buttonBox);
    primaryStage.setScene(new Scene(root, WINDOW_WIDTH, WINDOW_HEIGHT));
    primaryStage.show();
  }




  //============================================================================================
  //                               openFile()
  //
  // Called when the user clicks the "Select File" button.
  // This method displays a file chooser dialog that allows the user to browse folders
  //    to select a .txt or a .csv file.
  //
  // The selected file is opened in a BufferedReader.
  // The BufferedReader is assigned to the class variable BufferedReader reader.
  // The name of the selected file is assigned to the class variable String filename.
  //
  // Returns true if a file was successfully selected, opened and the BufferedReader
  // successfully created.
  // Otherwise, an error dialog is displayed and the method returns false.
  //============================================================================================
  private boolean openFile()
  {
    FileChooser fileChooser = new FileChooser();
    fileChooser.setTitle("Open Text File");
    fileChooser.setInitialDirectory(new File("."));
    fileChooser.getExtensionFilters().addAll(new ExtensionFilter("Text Files", "*.txt", "*.csv"));
    File selectedFile = fileChooser.showOpenDialog(null);
    if (selectedFile == null)
    {
      String fileName = "Fox.txt";
      try
      {
        reader = new BufferedReader(new FileReader("data/"+fileName));
      }
      catch (IOException e)
      { return false;
      }
      labelFileName.setText(fileName);
      buttonReadLine.setDisable(false);
    }

    else
    {
      try
      {
        reader = new BufferedReader(new FileReader(selectedFile));
        buttonReadLine.setDisable(false);
      }
      catch (IOException e)
      { showErrorDialog("IO Exception: " + e.getMessage());
        return false;
      }
      labelFileName.setText(selectedFile.getName());
    }


    textLineFields.setText("");
    lineCount = 0;
    return true;
  }



  //============================================================================================
  //                            readLineIntoFields(char delimiter)
  //
  // Reads the next line of file connected to BufferedReader reader.
  // The input delimiter is used to break the line read into a String array so that
  //   each element of the returned array is a field of the input line split at each
  //   occurrence of char delimiter.
  // For example, if the delimiter is " " and the line is "Hello there Joel!", then this
  //   method will return a String array with three elements:
  //         "Hello"
  //         "there"
  //         "Joel"
  //
  // Returns null if the last line of the file has already been read.
  //
  // This method will exit the program if a read error occurs.
  //============================================================================================
  private String[] readLineIntoFields(char delimiter)
  {
    String str = null;
    try
    {
      str = reader.readLine();
    }
    catch (IOException e)
    { showErrorDialog("readWordsOnLine(): IO Exception: " + e.getMessage());
      e.printStackTrace();
      System.exit(0);
    }


    if (str == null) return null;
    lineCount++;
    return  str.split(String.valueOf(delimiter));
  }



  //============================================================================================
  // Called by JavaFX when the user clicks a button.
  //============================================================================================
  @Override
  public void handle(ActionEvent event)
  {
    Object source = event.getSource();

    if (source == buttonSelectFile) openFile();
    else if (source == buttonReadLine) displayNextRecord();
  }



  //============================================================================================
  //                   displayNextRecord()
  // This method reads choiceBoxDelimiter to get the delimiter character.
  // It then reads the next file record into a String[] by calling
  //      String[] readLineIntoFields(char delimiter)
  //
  // If the array of fields is not null, then this method displays the record number,
  //    and each field of the record on a new line preceded by the field index.
  //
  // If the array of fields is null, then the end of file has been reached. In this case,
  //    an END OF FILE message is displayed and the read next record button is disabled.
  //============================================================================================
  private void displayNextRecord()
  {
    char delimiter = ' ';
    String selectedDelimiter = (String)choiceBoxDelimiter.getValue();
    if (selectedDelimiter.endsWith("Comma")) delimiter = ',';
    else if (selectedDelimiter.endsWith("Tab")) delimiter = '\t';

    String[] fieldList = readLineIntoFields(delimiter);

    if (fieldList == null)
    {
      textLineFields.setText("END OF FILE");
      buttonReadLine.setDisable(true);
      return;
    }

    String msg = "Record #" + lineCount + ":";
    for (int i=0; i<fieldList.length; i++)
    {
      msg = msg + "\n"+i + ") " + fieldList[i];
    }
    textLineFields.setText(msg);
  }

  //============================================================================================
  // Display a JavaFX Error Dialog.
  // This method will not return until the user clicks "ok" in the displayed dialog.
  //============================================================================================
  private void showErrorDialog(String msg)
  {
    Alert alert = new Alert(Alert.AlertType.ERROR);
    alert.setTitle("Error");
    alert.setContentText(msg);
    alert.showAndWait();
  }



  //============================================================================================
  // Program Entry point use to Launch JavaFX.
  // In this program, the command line arguments (args) are ignored.
  //============================================================================================
  public static void main(String[ ] args)
  {
    launch(args);
  }

}

 
