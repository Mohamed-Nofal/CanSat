/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ploto;





import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Label;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;


import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;

import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;





/**
 *
 * @author Mohamed
 */
public class FXMLDocumentController implements Initializable {
    
    ListView <String>lv;
    
     @FXML
     TextField tf;
    @FXML
    Label lb;
    
    @FXML
     TextField lx;

    @FXML
     TextField ly;

    @FXML
     TextField ln;
    
    
    @FXML
     TextField pre;
    @FXML
     BorderPane bp;
    
    ArrayList<point> p = new ArrayList<point>();
    @FXML
   public void brwase(ActionEvent e)    {
       FileChooser f1=new FileChooser();
       f1.getExtensionFilters().addAll(new ExtensionFilter("txt files","*.txt"));
       
      File f2=f1.showOpenDialog(null);
       
       if (f2!=null) {
           tf.setText(f2.getAbsolutePath());
           } 
       else{
       lb.setText("file not found");
       }
       

///////////////////////////////////////////////////////////

List<String> lines = Collections.emptyList();
    try {
      lines = Files.readAllLines(Paths.get(f2.getAbsolutePath()), StandardCharsets.UTF_8);
  
      
System.out.println();
      Scanner scannerA = new Scanner(f2);
      int lineNumber = 1;
    while(scannerA.hasNextLine())
    {
        String line = scannerA.nextLine();
        System.out.println(line);
        pre.setText(line);
//        p.set(lineNumber,p.add());
       
        lineNumber ++;
    }
     
        
    } 
    catch (IOException ex) {
      // TODO Auto-generated catch block
      
    }
       
   












///////////////////////////////
           
    
    
//           try {    
//        FileReader fr = new FileReader(""); 
////         create csvReader object and skip first Line 
//      CSVReader csvReader = new CSVReaderBuilder(fr).withSkipLines(1) .build(); 
//         
//        List<String[]> allData = csvReader.readAll(); 
// 
//        // print Data 
//        for (String[] row : allData) { 
//            for (String cell : row) { 
//                System.out.print(cell + "\t"); 
//            } 
//            System.out.println(); }
//        fr.close();
//       } catch(Exception ex) {
//      ex.getStackTrace();}
        
      
     
   }
       
       
       
       
       
       ////////////////////////////////
     
		
		
   
   

    
   
   
   
   
   
    @FXML
   public void  plott() {
       
       
      
       NumberAxis x=new NumberAxis(0,150,10);
       x.setLabel(lx.getText());
       NumberAxis y=new NumberAxis(0,100,10);
       y.setLabel(ly.getText());
       LineChart c=new LineChart(x, y);
       c.setTitle(ln.getText());
       
       
       
       XYChart.Series j=new XYChart.Series();
       j.setName("acceleration");
       j.getData().add(new XYChart.Data(10, 23));
        j.getData().add(new XYChart.Data(20, 14));
        j.getData().add(new XYChart.Data(30, 15));
        j.getData().add(new XYChart.Data(40, 24));
        j.getData().add(new XYChart.Data(50, 34));
        j.getData().add(new XYChart.Data(60, 36));
        j.getData().add(new XYChart.Data(70, 22));
        j.getData().add(new XYChart.Data(80, 45));
        j.getData().add(new XYChart.Data(90, 43));
       j.getData().add(new XYChart.Data(100, 17));
        j.getData().add(new XYChart.Data(110, 29));
        j.getData().add(new XYChart.Data(120, 25));
       
//           j.getData().addAll();
           c.getData().add(j);
           bp.setCenter(c);
       //////////////

		

   





// configuring everything by default
//		Plot plot = Plot.plot(null).
//			// setting data
//			series(null, Plot.data().
//				xy(1, 2).
//				xy(3, 4), null);
//		// saving sample_minimal.png
//		plot.save("sample_minimal", "png");
   }
   
   
   
   
   
   
   
  
  
   
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
        
      
	
    }    
  
    
}
