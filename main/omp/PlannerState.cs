﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace omp
{

    /// <summary>
    /// </summary>
    partial class Planner
    {

        /// <summary>
        /// </summary>
        private abstract class State
        {
            public virtual void onEnter(Planner planner) { }

            public virtual void onLeave(Planner planner) { }

            public virtual void onMainViewResize(Planner planner) { }

            public virtual void onPaint(Planner planner) { }

            public virtual void onMouseMove(Planner planner, Point point) { }

            public virtual void onMouseClick(Planner planner, Point point) { }

            public virtual void onTrackFrame(Planner planner, ListViewItem item) { }

            public virtual void updateSimulation(Planner planner) { }

            public virtual void renderMainView(Planner planner) { }
        }

        /// <summary>
        /// </summary>
        private class Stopped : State
        {
            public static readonly State Instance = new Stopped();

            static Stopped() { }

            private Stopped() { }
        }

        /// <summary>
        /// </summary>
        private class Starting : State
        {
            public static readonly State Instance = new Starting();

            static Starting() { }

            private Starting() { }

            // test {
            private void testPopulateListview(Planner planner, Lucid.Orbit.Frame frame)
            {
                ListViewItem item = new ListViewItem();
                item.Tag = frame.ID;
                item.SubItems.Add(frame.ID.ToString());
                item.SubItems.Add(frame.Name);
                item.SubItems.Add(frame.Description);
                item.StateImageIndex = 0;
                planner.orbitalFrameList.Items.Add(item);

                for (Lucid.Orbit.Frame child = frame.FirstChild; child != null; child = child.NextSibling)
                {
                    testPopulateListview(planner, child);
                }
            }
            // } test

            public override void onEnter(Planner planner)
            {
                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;

                unsafe { Lucid.GAL.Pipeline.initialize(clientSize.Width, clientSize.Height, 1, panel.Handle.ToPointer()); }

                // test {
                // initial user action should be to specify an ephemeris
                // or to have a default that is user specified.
                Lucid.Orbit.Ephemeris.Initialize("content/j2000.ephemeris");
                Lucid.Orbit.StarCatalog.Initialize("content/bsc5.starcatalog");

                planner.orbitalMechainics = new Lucid.Orbit.Mechanics(2451544.0);
                testPopulateListview(planner, planner.orbitalMechainics.Root);
                // } test

                planner.renderContext = new Lucid.GIGL.Context("content/render.context");

                planner.cameraFrame = new Lucid.Orbit.DynamicPoint(1001, "camera", "dynamic frame for camera");
                planner.orbitalMechainics.Attach(planner.orbitalMechainics.Root, planner.cameraFrame);
                planner.cameraFrame.RelativePosition = new Lucid.Math.Vector3(10, 10, 3);

                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;

                planner.camera = new Lucid.GIGL.Camera2D();
                planner.camera.InitPerspective(0.25f * 3.1415926f, planner.aspectRatio, 1, 1000);
                planner.camera.Look(new Lucid.Math.Vector3(20, 20, 15), new Lucid.Math.Vector3(0, 0, 0), new Lucid.Math.Vector3(0, 0, 1));

                planner.setMainMenuDefaults();
                planner.changeState(Editing.Instance);
            }
        }

        /// <summary>
        /// </summary>
        private class Editing : State
        {
            public static readonly State Instance = new Editing();

            static Editing() { }

            private Editing() { }

            public override void onMainViewResize(Planner planner)
            {
                SplitterPanel panel = planner.mainSplitter.Panel1;
                Size clientSize = panel.ClientSize;
                planner.aspectRatio = (float)clientSize.Width / (float)clientSize.Height;

                Lucid.GAL.Pipeline.resize(clientSize.Width, clientSize.Height);

                planner.camera.InitPerspective(0.25f * 3.1415926f, planner.aspectRatio, 1, 1000);
                planner.renderMainView();
            }

            public override void onPaint(Planner planner)
            {
                planner.renderMainView();
            }

            public override void onMouseMove(Planner planner, Point point)
            {
                Lucid.Orbit.Selection selection = planner.orbitalMechainics.Hit(point.X, point.Y);
                planner.statusLabel.Text = "TBD: fix this"; // selection.Description;
            }

            public override void onMouseClick(Planner planner, Point point)
            {
                Lucid.Orbit.Selection selection = planner.orbitalMechainics.Hit(point.X, point.Y);
                if (0 == selection.ID)
                    return;

                foreach(ListViewItem item in planner.orbitalFrameList.Items)
                {
                    if ((ulong)(item.Tag) == selection.ID)
                        item.Selected = true;
                }
            }

            public override void onTrackFrame(Planner planner, ListViewItem item)
            {
                if (planner.trackedFrameItem == item)
                {
                    int stateIndex = (item.StateImageIndex + 1) % 2;
                    item.StateImageIndex = stateIndex;

                    if (0 == stateIndex)
                        planner.trackedFrameItem = null;
                }
                else
                {
                    if (null != planner.trackedFrameItem)
                        planner.trackedFrameItem.StateImageIndex = 0;
                    planner.trackedFrameItem = item;
                    planner.trackedFrameItem.StateImageIndex = 1;
                }

                planner.orbitalMechainics.Detach(planner.cameraFrame);
                if (null != planner.trackedFrameItem)
                {
                    uint id = uint.Parse(planner.trackedFrameItem.SubItems[1].Text);
                    planner.trackedFrame = planner.orbitalMechainics[id];
                    planner.orbitalMechainics.Attach(planner.trackedFrame, planner.cameraFrame);
                }
                else
                {
                    planner.trackedFrame = null;
                    planner.orbitalMechainics.Attach(planner.orbitalMechainics.Root, planner.cameraFrame);
                }

                planner.cameraFrame.RelativePosition = new Lucid.Math.Vector3(0.5f, 0.5f, 0.1f);
            }

            public override void updateSimulation(Planner planner)
            {
                planner.orbitalMechainics.Update();
            }

            public override void renderMainView(Planner planner)
            {
                /// test {
                Lucid.Math.Vector3 position = planner.orbitalMechainics.InterpolatePosition(planner.cameraFrame);
                Lucid.Math.Vector3    focus = (null != planner.trackedFrame)
                    ? planner.orbitalMechainics.InterpolatePosition(planner.trackedFrame)
                    : position + new Lucid.Math.Vector3(10, 10, 10);
                planner.camera.Look(position, focus, new Lucid.Math.Vector3(0, 0, 1));
                planner.renderContext.Set(planner.camera);
                /// } test

                Lucid.GAL.Pipeline.beginScene();
                    planner.orbitalMechainics.Render(planner.renderContext);
                Lucid.GAL.Pipeline.endScene();
            }
        }

        /// <summary>
        /// </summary>
        private class Stopping : State
        {
            public static readonly State Instance = new Stopping();

            static Stopping() { }

            private Stopping() { }

            public override void onEnter(Planner planner)
            {
                planner.orbitalMechainics.Shutdown();
                
                Lucid.Orbit.Ephemeris.Shutdown();
                Lucid.Orbit.StarCatalog.Shutdown();

                planner.changeState(Stopped.Instance);
            }
        }

    }   // planner
}       // omp
