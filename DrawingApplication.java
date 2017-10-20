/*
 * Programming Assignment 5 Drawing Application
 * Implemented slightly differently from the book's implementation, still works the same.
 * @author Alexander Vary
 * ajv5266
 * October 19th, 2017
 */
package drawingapplication;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;

public class DrawingApplication extends JFrame
{

    private final JButton undoButton;
    private final JButton clearButton;
    private final JLabel shapeLabel;
    private final JComboBox<String> shapeSelect;
    private static final String[] shapes = {"Line", "Oval", "Rectangle"};
    private final JCheckBox filledOption;
    private final JCheckBox gradientOption;
    private final JButton colorOneButton;
    private final JButton colorTwoButton;
    private final JLabel widthLabel;
    private final JTextField widthText;
    private final JLabel dashedLabel;
    private final JTextField dashedText;
    private final JCheckBox dashedOption;
    private final DrawingJPanel drawingArea;
    private final JLabel mousePosition;
    private Color colorOne, colorTwo;
    private boolean DrawRectBool, DrawOvalBool;
    private Stroke lineSettings;
    private int startX, startY, endX, endY;
    private java.util.List<ShapesDrawn> shapeList;
    
    DrawingApplication()
    {
        super("Guess the Number Game");
        colorOne = Color.BLACK;
        colorTwo = Color.WHITE;
        DrawRectBool = false;
        DrawOvalBool = false;
        startX = -100;
        startY = -100;
        endX = -100;
        endY = -100;
        shapeList = new ArrayList<>();
        
        JPanel topOptions = new JPanel( new FlowLayout());
        undoButton = new JButton("Undo");
        undoButton.addActionListener(new UndoHandler());
        clearButton = new JButton("Clear");
        clearButton.addActionListener(new ClearHandler());
        shapeLabel = new JLabel(" Shapes: ");
        shapeSelect = new JComboBox(shapes);
        shapeSelect.addActionListener(new ShapeSelectionHandler());
        filledOption = new JCheckBox("Filled");
        topOptions.add(undoButton);
        topOptions.add(clearButton);
        topOptions.add(shapeLabel);
        topOptions.add(shapeSelect);
        topOptions.add(filledOption);
        
        JPanel bottomOptions = new JPanel( new FlowLayout());
        gradientOption = new JCheckBox("Use Gradient");
        colorOneButton = new JButton("1st Color");
        colorOneButton.addActionListener(new ColorOneSelectionHandler());
        colorTwoButton = new JButton("2nd Color");
        colorTwoButton.addActionListener(new ColorTwoSelectionHandler());
        
        widthLabel = new JLabel(" Line Width: ");
        widthText = new JTextField("10", 3);
        
        dashedLabel = new JLabel(" Dash Length: ");
        dashedText = new JTextField("10", 3);
        
        dashedOption = new JCheckBox("Dashed");
        
        bottomOptions.add(gradientOption);
        bottomOptions.add(colorOneButton);
        bottomOptions.add(colorTwoButton);
        bottomOptions.add(widthLabel);
        bottomOptions.add(widthText);
        bottomOptions.add(dashedLabel);
        bottomOptions.add(dashedText);
        bottomOptions.add(dashedOption);
        
        JPanel drawingAreaLayout = new JPanel();
        drawingArea = new DrawingJPanel();
        drawingArea.setBackground(Color.WHITE);
        drawingArea.setPreferredSize(new Dimension(650, 350));
        drawingArea.addMouseListener(new DrawingStartHandler());
        drawingArea.addMouseMotionListener(new DrawingHandler());
        drawingAreaLayout.add(drawingArea);
        
        JPanel mouseLabelLayout = new JPanel( new FlowLayout(FlowLayout.LEFT));
        mousePosition = new JLabel("(0,0)");
        
        mouseLabelLayout.add(mousePosition);
        
        getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
        
        add(topOptions, getContentPane());
        add(bottomOptions, getContentPane());
        add(drawingAreaLayout, getContentPane());
        add(mouseLabelLayout, getContentPane());
        
        drawingArea.addMouseMotionListener(new CoordinateHandler());
    }
    
    //A JPanel where everything can be drawn must be made. This handles the current drawings and all previous drawings.
    private class DrawingJPanel extends JPanel
    {
        @Override
        public void paintComponent(Graphics g)
        {
            super.paintComponent(g);
            Graphics2D g2d = (Graphics2D) g.create();
            
            //The following loop adds all previous shapes to the JPanel in quick succession.
            for (ShapesDrawn i : shapeList)
            {
                if (i.getShapeType() == 0)
                {
                    if (i.getDash())
                    {
                        float dashArray[] = {i.getDashLength()};
                        lineSettings = new BasicStroke(i.getLineWidth(), BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 0, dashArray, 0);
                    }
                    else
                        lineSettings = new BasicStroke(i.getLineWidth(), BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 0);
                    
                    if (i.getGradient())
                    {
                        GradientPaint gradient = new GradientPaint(0, 0, i.getColorOne(), 50, 50, i.getColorTwo(), true);
                        g2d.setPaint(gradient);
                    }
                    else
                    {
                        g2d.setColor(i.getColorOne());
                    }
                    
                    g2d.setStroke(lineSettings);
                    g2d.drawLine(i.getStartX(), i.getStartY(), i.getEndX(), i.getEndY());
                }
                else if (i.getShapeType() == 1)
                {
                    if (i.getGradient())
                    {
                        GradientPaint gradient = new GradientPaint(0, 0, i.getColorOne(), 50, 50, i.getColorTwo(), true);
                        g2d.setPaint(gradient);
                    }
                    else
                    {
                        g2d.setColor(i.getColorOne());
                    }
                    
                    if (i.getFill())
                        g2d.fillOval(Math.min(i.getStartX(), i.getEndX()), Math.min(i.getStartY(),
                                i.getEndY()), Math.abs(i.getEndX() - i.getStartX()), Math.abs(i.getEndY() - i.getStartY()));
                    else
                    {
                        if (i.getDash())
                        {
                            float dashArray[] = {i.getDashLength()};
                            g2d.setStroke(new BasicStroke(i.getLineWidth(), BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 0, dashArray, 0));
                        }
                        else
                        {
                            g2d.setStroke(new BasicStroke(i.getLineWidth()));
                        }
                        g2d.drawOval(Math.min(i.getStartX(), i.getEndX()), Math.min(i.getStartY(),
                                i.getEndY()), Math.abs(i.getEndX() - i.getStartX()), Math.abs(i.getEndY() - i.getStartY()));
                    }
                }
                else
                {
                    if (i.getGradient())
                    {
                        GradientPaint gradient = new GradientPaint(0, 0, i.getColorOne(), 50, 50, i.getColorTwo(), true);
                        g2d.setPaint(gradient);
                    }
                    else
                    {
                        g2d.setColor(i.getColorOne());
                    }
                    
                    if (i.getFill())
                        g2d.fillRect(Math.min(i.getStartX(), i.getEndX()), Math.min(i.getStartY(),
                                i.getEndY()), Math.abs(i.getEndX() - i.getStartX()), Math.abs(i.getEndY() - i.getStartY()));
                    else
                    {
                        if (i.getDash())
                        {
                            float dashArray[] = {i.getDashLength()};
                            g2d.setStroke(new BasicStroke(i.getLineWidth(), BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 0, dashArray, 0));
                        }
                        else
                        {
                            g2d.setStroke(new BasicStroke(i.getLineWidth()));
                        }
                        g2d.drawRect(Math.min(i.getStartX(), i.getEndX()), Math.min(i.getStartY(),
                                i.getEndY()), Math.abs(i.getEndX() - i.getStartX()), Math.abs(i.getEndY() - i.getStartY()));
                    }
                }
            }
            
            if (gradientOption.isSelected())
            {
                GradientPaint gradient = new GradientPaint(0, 0, colorOne, 50, 50, colorTwo, true);
                g2d.setPaint(gradient);
            }
            else
               g2d.setColor(colorOne); 
            
            ShapesDrawn comparator;
            
            if (!shapeList.isEmpty())
                comparator = shapeList.get(shapeList.size() - 1);
            else
                comparator = new ShapesDrawn();
            
            //The following comparisons are done because paintComponent would sometimes run again when a
            //button was pressed, copying the previous shape with changed attributes.
            if(startX == comparator.getStartX() && startY == comparator.getStartY()
                    && endX == comparator.getEndX() && endY == comparator.getEndY()) {}
            else
            {
                if (DrawRectBool)
                {
                    if (filledOption.isSelected())
                    {
                        g2d.fillRect(Math.min(startX, endX), Math.min(startY, endY), Math.abs(endX - startX), Math.abs(endY - startY));
                    }
                    else
                    {
                        g2dDrawMaker(g2d).drawRect(Math.min(startX, endX), Math.min(startY, endY), Math.abs(endX - startX), Math.abs(endY - startY));
                    }
                }
                else if (DrawOvalBool)
                {
                    if (filledOption.isSelected())
                    {
                        g2d.fillOval(Math.min(startX, endX), Math.min(startY, endY), Math.abs(endX - startX), Math.abs(endY - startY));
                    }
                    else
                    {
                        g2dDrawMaker(g2d).drawOval(Math.min(startX, endX), Math.min(startY, endY), Math.abs(endX - startX), Math.abs(endY - startY));
                    }
                }
                else
                {    
                    String widthInput = widthText.getText();
                    int width = Integer.parseInt(widthInput);

                    if (dashedOption.isSelected())
                    {
                        String dashedInput = dashedText.getText();
                        int dash = Integer.parseInt(dashedInput);
                        float dashArray[] = {dash};
                        lineSettings = new BasicStroke(width, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 0, dashArray, 0);
                    }
                    else
                    {
                        lineSettings = new BasicStroke(width, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 0);
                    }
                    g2d.setStroke(lineSettings);    
                    g2d.drawLine(startX, startY, endX, endY);
                }
            }
        }
    }
    
    //This method sets up the dashed line of a drawRect or drawOval.
    public Graphics2D g2dDrawMaker(Graphics2D g2d)
    {
        String widthInput = widthText.getText();
        int width = Integer.parseInt(widthInput);

        if (dashedOption.isSelected())
        {
            String dashedInput = dashedText.getText();
            int dash = Integer.parseInt(dashedInput);
            float dashArray[] = {dash};
            g2d.setStroke(new BasicStroke(width, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 0, dashArray, 0));
        }
        else
        {
            g2d.setStroke(new BasicStroke(width));
        }
        
        return g2d;
    }
    
    //This is "MyShape." All information about the shape at the time of its creation is stored here.
    private class ShapesDrawn
    {
        private final int startX, startY, endX, endY, shapeType, lineWidth, dashLength;
        private final boolean dash, fill, grad;
        private final Color color1, color2;
        //For shapeType, 0 = line, 1 = oval, 2 = rectangle
        
        ShapesDrawn()
        {
            this(0,0,0,0,0,Color.BLACK,Color.WHITE,10,10,false,false,false);
        }
        
        ShapesDrawn(int startX, int startY, int endX, int endY, int shapeType, Color color1, Color color2,
                int lineWidth, int dashLength, boolean fill, boolean dash, boolean grad)
        {
            this.startX = startX;
            this.startY = startY;
            this.endX = endX;
            this.endY = endY;
            this.shapeType = shapeType;
            this.color1 = color1;
            this.color2 = color2;
            this.lineWidth = lineWidth;
            this.dashLength = dashLength;
            this.dash = dash;
            this.fill = fill;
            this.grad = grad;
        }
        
        public int getStartX()
        {
            return startX;
        }
        
        public int getStartY()
        {
            return startY;
        }
        
        public int getEndX()
        {
            return endX;
        }
        
        public int getEndY()
        {
            return endY;
        }
        
        public int getShapeType()
        {
            return shapeType;
        }
        
        public Color getColorOne()
        {
            return color1;
        }
        
        public Color getColorTwo()
        {
            return color2;
        }
        
        public int getLineWidth()
        {
            return lineWidth;
        }
        
        public int getDashLength()
        {
            return dashLength;
        }
        
        public boolean getFill()
        {
            return fill;
        }
        
        public boolean getDash()
        {
            return dash;
        }
        
        public boolean getGradient()
        {
            return grad;
        }
    }
    
    //Handler section begins
    private class UndoHandler implements ActionListener
    {
        @Override
        public void actionPerformed (ActionEvent e)
        {
            if (!shapeList.isEmpty())
            {
                shapeList.remove(shapeList.size() - 1);
                //These numbers are high so that when repaint happens, any possible shape created is far off screen.
                startX = 10000;
                startY = 10000;
                endX = 10000;
                endY = 10000;
                drawingArea.repaint();
            }
        }
    }
    
    private class ClearHandler implements ActionListener
    {
        @Override
        public void actionPerformed (ActionEvent e)
        {
            if (!shapeList.isEmpty())
            {
                shapeList.clear();
                startX = 10000;
                startY = 10000;
                endX = 10000;
                endY = 10000;
                drawingArea.repaint();
            }
        }
    }
    
    private class ShapeSelectionHandler implements ActionListener
    {
        @Override
        public void actionPerformed (ActionEvent e)
        {
            String shape = (String)shapeSelect.getSelectedItem();
            
            if (shape == "Line")
            {
                DrawRectBool = false;
                DrawOvalBool = false;
            }
            else if (shape == "Oval")
            {
                DrawRectBool = false;
                DrawOvalBool = true;
            }
            else
            {
                DrawRectBool = true;
                DrawOvalBool = false;
            }
        }
    }
    
    private class ColorOneSelectionHandler implements ActionListener
    {
        @Override
        public void actionPerformed (ActionEvent e)
        {
            Color colorHolder = colorOne;
            colorOne = JColorChooser.showDialog(DrawingApplication.this, "Select Color 1", colorOne);
            
            if (colorOne == null)
                colorOne = colorHolder;
        }
    }
    
    private class ColorTwoSelectionHandler implements ActionListener
    {
        @Override
        public void actionPerformed (ActionEvent e)
        {
            Color colorHolder = colorTwo;
            colorTwo = JColorChooser.showDialog(DrawingApplication.this, "Select Color 2", colorTwo);
            
            if (colorTwo == null)
                colorTwo = colorHolder;
        }
    }
    
    private class DrawingStartHandler implements MouseListener
    {
        @Override
        public void mouseExited( MouseEvent e ) {}

        @Override
        public void mouseEntered( MouseEvent e ) {}

        @Override
        public void mouseReleased( MouseEvent e )
        {
            ShapesDrawn shapeHolder;
            String widthInput = widthText.getText();
            int width = Integer.parseInt(widthInput);
            String dashInput = dashedText.getText();
            int dash = Integer.parseInt(dashInput);
            int type;

            if (DrawRectBool)
                type = 2;
            else if (DrawOvalBool)
                type = 1;
            else
                type = 0;

            shapeHolder = new ShapesDrawn(startX, startY, endX, endY, type, colorOne, colorTwo,
                    width, dash, filledOption.isSelected(), dashedOption.isSelected(), gradientOption.isSelected());
            shapeList.add(shapeHolder);
        }

        @Override
        public void mousePressed( MouseEvent e )
        {
            startX = e.getX();
            startY = e.getY();
        }

        @Override
        public void mouseClicked( MouseEvent e ) {}
    }
    
    private class DrawingHandler implements MouseMotionListener
    {
        @Override
        public void mouseMoved( MouseEvent e ) {}

        @Override
        public void mouseDragged( MouseEvent e )
        {
            endX = e.getX();
            endY = e.getY();
            repaint(); 
        }
    }
    
    private class CoordinateHandler implements MouseMotionListener
    {
        @Override
        public void mouseMoved( MouseEvent e )
        {		
          mousePosition.setText("(" + e.getX() + "," + e.getY() + ")");
        }

        @Override
        public void mouseDragged( MouseEvent e )
        {
            mouseMoved(e);
        }
    }
    
    public static void main(String[] args)
    {
        DrawingApplication draw = new DrawingApplication();
        draw.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        draw.setSize(650, 525);
        draw.setVisible(true);
        draw.setResizable(false);
    }
}
