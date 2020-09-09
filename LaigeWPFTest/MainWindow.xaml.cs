// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace LaigeWPFTest
{

    struct TrackMapData
    {
        public float X;
        public float Y;
        public float Speed;
    }
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
   
        private LaigeRenderer renderer;

        // The C++ graphics debugger has trouble to function when executed within a WPF window.
        // This means we need to make it record manually. To do this, we need something (here, a button) to tells it when a frame should get recorded.
        private bool graphicsDebuggerEnabled;
        private bool graphicsDebuggerKeyDown;

        private const float edgeThreshold = 50;

        List<TrackMapData> data;
        LaigeRenderer.LaigeLine[] lines;

        float minX, minY, maxX, maxY;

        private LaigeRenderer.LaigeColor[] rainbowDefs = new LaigeRenderer.LaigeColor[]
        {
            new LaigeRenderer.LaigeColor(1,0,0,1),
            new LaigeRenderer.LaigeColor(1,0.5f,0,1),
            new LaigeRenderer.LaigeColor(1,1,0,1),
            new LaigeRenderer.LaigeColor(0.5f,1,0,1),
            new LaigeRenderer.LaigeColor(0,1,0,1),
            new LaigeRenderer.LaigeColor(0,1,0.75f,1),
            new LaigeRenderer.LaigeColor(0,0.5f,1,1),
            new LaigeRenderer.LaigeColor(0,0,1,1),
      
        };

        public MainWindow()
        {
            InitializeComponent();


            graphicsDebuggerEnabled = false;
            graphicsDebuggerKeyDown = false;
            LoadTrackMapData();
            Loaded += MainWindow_Loaded;
            CompositionTarget.Rendering += CompositionTarget_Rendering;
            
        }

        private void LoadTrackMapData()
        {
            data = new List<TrackMapData>();
            lines = new LaigeRenderer.LaigeLine[1];
            using (var stream = File.OpenRead("trackmapdata.bin"))
            {
                BinaryReader rdr = new BinaryReader(stream);
                while(rdr.BaseStream.Position != rdr.BaseStream.Length)
                {
                    float x = rdr.ReadSingle();
                    float y = rdr.ReadSingle();
                    float spd = rdr.ReadSingle();
                    data.Add(new TrackMapData() { X = x, Y = y, Speed = spd });
                }

                data.Add(data[0]);
                data.Add(data[1]);
            }

            var topSpeed = data.Max(x => x.Speed);
            var minSpeed = data.Min(x => x.Speed);

            minX = data.Min(x => x.X);
            minY = data.Min(x => x.Y);

            maxX = data.Max(x => x.X);
            maxY = data.Max(x => x.Y);

            // Make all values positive for easier math.
            for (int i = 0; i < data.Count; ++i)
                data[i] = new TrackMapData() { X = data[i].X - minX, Y = data[i].Y - minY,Speed = data[i].Speed };

            minX = data.Min(x => x.X);
            minY = data.Min(x => x.Y);

            maxX = data.Max(x => x.X);
            maxY = data.Max(x => x.Y);

            LaigeRenderer.LaigeLinePoint[] pnts = new LaigeRenderer.LaigeLinePoint[data.Count];
            for (int i = 0; i < pnts.Length;++i)
            {
                LaigeRenderer.LaigeLinePoint pnt = new LaigeRenderer.LaigeLinePoint();

                float speedT = (data[i].Speed / (topSpeed));
                int colorIndex = (rainbowDefs.Length)-( speedT == 1?rainbowDefs.Length: (int)((rainbowDefs.Length+1) * speedT));
                
                pnt.Color = rainbowDefs[colorIndex];
                pnt.Thickness = 7;
                pnt.Position = new LaigeRenderer.LaigePoint(data[i].X, data[i].Y);
                pnts[i] = pnt;
            }

            lines[0] = new LaigeRenderer.LaigeLine((uint)pnts.Length, pnts);

          
        }



        private void img_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            double dpiScale = 1.0; // default value for 96 dpi
            renderer.SizeChanged(e.NewSize.Width, e.NewSize.Height, dpiScale);
            Draw();
        }


        private void CompositionTarget_Rendering(object sender, EventArgs e)
        {
           //Draw(); // if enabled, updates at window refresh rate (usually monitor refresh rate, so 60 HZ upwards).
        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
         
            renderer = new LaigeRenderer(1, ActualWidth, ActualHeight, new System.Windows.Interop.WindowInteropHelper(this).Handle);
            img.Source = renderer.D3D11Image;
     
        }

        private void Draw()
        {
            if (renderer == null)
                return;


            LaigeRenderer.LaigeRect rect = new LaigeRenderer.LaigeRect() { X = -edgeThreshold, Y = -edgeThreshold,
                Height = (float)(maxX / (img.ActualWidth/img.ActualHeight)) +(edgeThreshold*2), Width = maxX+(edgeThreshold*2) };
            renderer.SetData(lines,rect);

            renderer.BeginDraw(graphicsDebuggerEnabled);
            graphicsDebuggerEnabled = false;

        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.C && !graphicsDebuggerKeyDown)
            {    
                graphicsDebuggerEnabled = true;
                graphicsDebuggerKeyDown = true;
            }
        }

        private void Window_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.C)
            {
                graphicsDebuggerKeyDown = false;
            }
        }

   
    }
}
