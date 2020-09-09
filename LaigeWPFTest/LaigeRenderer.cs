// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

using Microsoft.Wpf.Interop.DirectX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LaigeWPFTest
{

    /// <summary>
    /// 
    /// </summary>
    public class LaigeRenderer
    {

        [DllImport("laige.dll")]
        static extern void _Initialize();

        [DllImport("laige.dll")]
        static extern void _SetScreenMatrix(LaigeRect[] bounds);

        [DllImport("laige.dll")]
        static extern void _BeginDraw(IntPtr renderTarget, bool isNewSurface, bool enableGraphicsDebugger);

        [DllImport("laige.dll")]
        static extern void _EndDraw();

        [DllImport("laige.dll")]
        static extern void _DrawLine1(LaigePoint[] points, uint length, float thickness, LaigeColor color, LaigeRect[] specialBounds);

        [DllImport("laige.dll")]
        static extern void _DrawLine2(LaigeLine points, LaigeRect[] specialBounds);

        [DllImport("laige.dll")]
        static extern void _DrawLines(LaigeLine[] lines, uint length, LaigeRect[] specialBounds);

        [DllImport("laige.dll")]
        static extern void _DrawDebugTriangle();

        private D3D11Image d3d11Image;
        private bool enableGraphicsDebugger;
    
        public D3D11Image D3D11Image => d3d11Image;

        public LaigeRenderer(double dpi, double width, double height, IntPtr owner)
        {
            _Initialize();

            d3d11Image = new D3D11Image();
            d3d11Image.WindowOwner = owner;

            enableGraphicsDebugger = false;

            SizeChanged(width,height,dpi);
            d3d11Image.OnRender += Draw;
           
        }

        public void BeginDraw(bool enableGraphicsDebugger = false)
        {
            this.enableGraphicsDebugger = enableGraphicsDebugger;
            d3d11Image.RequestRender();
        }


  
        public void DrawLines(LaigeLine[] lines, uint length, LaigeRect[] specialBounds)
        {
            _DrawLines(lines, length, specialBounds);
        }

        public void EndDraw()
        {
            _EndDraw();
        }


        private LaigeLine[] lines;
        private LaigeRect rect;
        public void SetData(LaigeLine[] lines, LaigeRect laigeRect)
        {
            this.lines = lines;
            this.rect = laigeRect;
        }

        void Draw(IntPtr renderTarget, bool isNewSurface)
        {
      
            _BeginDraw(renderTarget, isNewSurface, enableGraphicsDebugger);

            if (lines != null)
                _DrawLines(lines, (uint)lines.Length, new LaigeRenderer.LaigeRect[] { rect });
            //   new LaigeRenderer.LaigeRect[] { new LaigeRenderer.LaigeRect() { X = -1100, Y = -800, Width = 1700, Height = 1700 } });
            _EndDraw();

         
 
        }

        public void SizeChanged(double w, double h, double dpi)
        {
            int width = (int)(w< 0 ? 0 : Math.Ceiling(w * dpi));
            int height = (int)(h < 0 ? 0 : Math.Ceiling(h * dpi));
            d3d11Image.SetPixelSize(width, height);

            var tt = new LaigeRect() { Height = height, Width = width };
            _SetScreenMatrix(new LaigeRect[] { tt });
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct LaigePoint
        {
            public float X;
            public float Y;

            public LaigePoint(float x, float y)
            {
                X = x;
                Y = y;
            }


        }

        [StructLayout(LayoutKind.Sequential)]
        public struct LaigeRect
        {
            public float X;
            public float Y;
            public float Width;
            public float Height;


        }

        [StructLayout(LayoutKind.Sequential)]
        public struct LaigeColor
        {
            public float R;
            public float G;
            public float B;
            public float A;

            public LaigeColor(float r, float g, float b, float a)
            {
                R = r;
                G = g;
                B = b;
                A = a;
            }

        }

        [StructLayout(LayoutKind.Sequential)]
        public struct LaigeLinePoint
        {

            public LaigePoint Position;
            public LaigeColor Color;
            public float Thickness;
        };

        /// <summary>
        /// Represents a LaigeLine.
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct LaigeLine
        {
            public uint Length;
            public IntPtr Points;
            static GCHandle gc;

            public LaigeLine(uint length, LaigeLinePoint[] points)
            {
                this.Length = length;

                gc = GCHandle.Alloc(points, GCHandleType.Pinned); // Pinned is important or the object can be moved by the GC.
                Points = gc.AddrOfPinnedObject();

            }
        };


    }
}

