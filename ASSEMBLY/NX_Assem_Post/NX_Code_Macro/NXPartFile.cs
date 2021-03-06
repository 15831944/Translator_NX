﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NXOpen;
using NXOpen.Positioning;

namespace NX_Code_Macro
{
    class NXPartFile
    {
        //private static Session nxSession;
        #region
        private static Part Stpfile, Assem;
        private static PartLoadStatus partloadstatus;
        private static BodyCollection tempbody;
        private static Body[] pBody;
        private static Face[] facenameid;
        private static Edge[] tempedgeid, edgeid;
        private static Point3d[] FaceEdgeVrtx, UFaceEdgeVrtx, EdgeVrtx;

        private static Matrix3x3[] OldPartOrntn;

        private static ComponentPositioner nxPositioner;
        private static Constraint[] nxConstraints;

        private static int lenBody, lenFace, lenEdge, lenVertx, numofFacesint, numofFacesint1, h, e1;
        private static string[] componentname, facename, edgenames;


        Dictionary<string, int> NumOfBodz = new Dictionary<string, int>();

        Dictionary<int, string> FeatureNames = new Dictionary<int, string>();
        Dictionary<int, string> EdgeNames = new Dictionary<int, string>();
        Dictionary<int, Point3d[]> EdgeVrtxInt = new Dictionary<int, Point3d[]>();
        Dictionary<int, Point3d[]> PartVrtxInt = new Dictionary<int, Point3d[]>();
        Dictionary<Point3d[], string> VrtxFaceName = new Dictionary<Point3d[], string>();
        Dictionary<string, string[]> Feature2FaceNames = new Dictionary<string, string[]>();
        Dictionary<string, string[]> Feature2EdgeNames = new Dictionary<string, string[]>();
        #endregion

        public void FileLoad(Session nxSession, string FileDirectory)
        {

            Stpfile = nxSession.Parts.OpenDisplay(FileDirectory, out partloadstatus);
            Stpfile = nxSession.Parts.Work;
            //Stpfile.CoordinateSystems.CreateCoordinateSystem(
            tempbody = nxSession.Parts.Work.Bodies;
            string name = Stpfile.JournalIdentifier.ToString();
            Console.WriteLine(name);
            pBody = tempbody.ToArray();
            lenBody = pBody.Length;
            NXOpen.Features.Feature[] tempfeature;
            NumOfBodz.Add(FileDirectory, lenBody);//= new string[100];
            e1 = 0;
            h = 0;
            for (int y = 0; y < lenBody; y++)
            {
                facenameid = pBody[y].GetFaces();
                tempfeature = pBody[y].GetFeatures();
                string[] featurenames = new string[tempfeature.Length];
                Edge[] edgesinbody = pBody[y].GetEdges();
                EdgeVrtx = new Point3d[2];
                edgenames = new string[edgesinbody.Length];
                for (int r = 0; r < edgesinbody.Length; r++)
                {
                    EdgeVrtx = new Point3d[2];
                    edgenames[r] = edgesinbody[r].JournalIdentifier.ToString();
                    Point3d a, b;
                    edgesinbody[r].GetVertices(out a, out b);
                    Point3d[] tempEdgeVrtx = new Point3d[2];
                    tempEdgeVrtx[0] = a;
                    tempEdgeVrtx[1] = b;
                    EdgeVrtx = tempEdgeVrtx.Distinct().ToArray();
                    EdgeVrtxInt.Add(e1, EdgeVrtx);
                    EdgeNames.Add(e1, edgenames[r]);
                    e1++;
                }

                lenFace = facenameid.Length;
                facename = new string[lenFace];
                for (int x = 0; x < lenFace; x++)
                {
                    facename[x] = facenameid[x].JournalIdentifier.ToString();
                    ///Dictionary Additon;
                    edgeid = facenameid[x].GetEdges();
                    lenEdge = edgeid.Length;
                    lenVertx = 2 * lenEdge;
                    FaceEdgeVrtx = new Point3d[lenVertx];
                    int w2 = 0;
                    for (int w1 = 0; w1 < lenEdge; w1++)
                    {
                        for (; w2 < lenVertx; )
                        {
                            Point3d a, b;
                            edgeid[w1].GetVertices(out a, out b);
                            FaceEdgeVrtx[w2] = a;
                            FaceEdgeVrtx[w2 + 1] = b;
                            w2 = w2 + 2;
                            break;
                        }
                    }
                    UFaceEdgeVrtx = FaceEdgeVrtx.Distinct().ToArray();
                    PartVrtxInt.Add(h, UFaceEdgeVrtx);
                    h++;
                    VrtxFaceName.Add(UFaceEdgeVrtx, facename[x]);
                }
                for (int f = 0; f < tempfeature.Length; f++)
                {
                    featurenames[f] = tempfeature[f].JournalIdentifier.ToString();
                }
                FeatureNames.Add(y, featurenames[0]);
                Feature2FaceNames.Add(featurenames[0], facename);
                Feature2EdgeNames.Add(featurenames[0], edgenames);
            }
            NXOpen.PartCloseResponses partCloseResponse1;
            partCloseResponse1 = nxSession.Parts.NewPartCloseResponses();
            Stpfile.Close(BasePart.CloseWholeTree.False, NXOpen.BasePart.CloseModified.UseResponses, partCloseResponse1);
            Console.WriteLine("It works");
        }
        public int NumOfFaces()
        {
            return h;
        }
        public int NumOfEdges()
        {
            return e1;
        }
        public int BodzinPart(string partname)
        {
            int x = 0;
            x = NumOfBodz[partname];
            return x;
        }
        public string featurename(int x)
        {
            return FeatureNames[x];
        }
        public string[] faceinfeature(string y)
        {
            return Feature2FaceNames[y];
        }
        public string[] EdgeinFeature(string y)
        {
            return Feature2EdgeNames[y];
        }
        public string FaceName(Point3d[] x)
        {
            string y;
            y = VrtxFaceName[x];
            return y;
        }
        public Point3d[] FaceVrtx(int c)
        {
            Point3d[] d = PartVrtxInt[c];
            return d;
        }
        public string EdgeName(int x)
        {
            string y;
            y = EdgeNames[x];
            return y;
        }
        public Point3d[] EdgeMVrtx(int c)
        {
            Point3d[] d = EdgeVrtxInt[c];
            return d;
        }
    }
}
