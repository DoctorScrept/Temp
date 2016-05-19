using UnityEngine;
using System.Collections;

public class Plotter : MonoBehaviour {

	public MeshFilter[] slots;
	public MeshFilter f;

	void Start () {
		Surface s = new Surface();
		f.mesh = Generate(s.heightMap, s.GetLengthX(), s.GetLengthY(), s.GetMaxHeight());
//		f.mesh = GenerateMesh(new Surface());
	}

//	void Update () {}

	public void DrawToSlot(int slotId, Surface surface)
	{
		slots[slotId].mesh = GenerateMesh(surface);
	}

	private Color GetColorByHeight(float height) {
		return Color.Lerp(Color.red, Color.green, height);
	}
	
	public Mesh GenerateMesh(Surface surface)
	{
		int numVertexes = surface.GetLength();
		int numTrianglesPoints = (surface.GetLengthX() - 1) * (surface.GetLengthY() - 1) * 6;
		int sizeX = surface.GetLengthX();
		int sizeY = surface.GetLengthY();

		Vector3[] vertices = new Vector3[numVertexes];
		Color[] colors = new Color[numVertexes];

		for (int i = 0; i < numVertexes; i++)
		{
			vertices[i] = surface.GetPoint(i);
			colors[i] = GetColorByHeight(surface.GetHeight(i) / surface.GetMaxHeight());
		}

		int j = 0;
		int[] triangles = new int[numTrianglesPoints];
		for (int x = 0; x < sizeX - 1; x++)
		{
			for (int y = 0; y < sizeY - 1; y++)
			{
				int firtsQuadId = y * sizeX + x;

				triangles[j++] = firtsQuadId;
				triangles[j++] = firtsQuadId + sizeX;
				triangles[j++] = firtsQuadId + sizeX + 1;

				triangles[j++] = firtsQuadId;
				triangles[j++] = firtsQuadId + sizeX + 1;
				triangles[j++] = firtsQuadId + 1;
			}
		}

		return surface.mesh = new Mesh () {
			vertices = vertices,
			colors = colors,
			triangles = triangles
		};
	}
	

	// Old version, jast as example
	private Mesh Triangle(Vector3 vertex0, Vector3 vertex1, Vector3 vertex2, float max)
	{
		var normal = Vector3.Cross((vertex1 - vertex0), (vertex2 - vertex0)).normalized;
		var mesh = new Mesh
		{
			vertices = new [] {vertex0, vertex1, vertex2},
			normals = new [] {normal, normal, normal},
			uv = new [] {new Vector2(0, 0), new Vector2(0, 1), new Vector2(1, 1)},
			triangles = new [] {0, 1, 2},
			colors = new [] {	GetColorByHeight(vertex0.z / max), 
								GetColorByHeight(vertex1.z / max), 
								GetColorByHeight(vertex2.z / max)}
		};
		return mesh;
	}

	// Old version, jast as example
	public Mesh Generate(float[,] map, int sizeX, int sizeY, float max)
	{
		var combine = new CombineInstance[(sizeX - 1) * (sizeY -1) * 2];
		
		var i = 0;
		for (int x = 0; x < sizeX -1; x++)
		{
			for (int y = 0; y < sizeY - 1; y++)
			{
				combine[i].mesh = Triangle(new Vector3(x, y, map[x, y]), 
				                           new Vector3(x, y+1, map[x, y+1]), 
				                           new Vector3(x+1, y+1, map[x+1, y+1]), max);
				i++;
				
				combine[i].mesh = Triangle(new Vector3(x, y, map[x, y]), 
				                           new Vector3(x+1, y+1, map[x+1, y+1]), 
				                           new Vector3(x+1, y, map[x+1, y]), max);
				i++;
			}
		}
		
		var mesh = new Mesh();
		mesh.CombineMeshes(combine, true, false);
		return mesh;
	}
}


public class Surface
{
	public int x = 4;
	public int y = 4;

	public float[,] heightMap = { 	
		{1f, 0f, 0f, 0f},
		{0f, 2f, 0f, 0f},
		{0f, 0f, 0f, 0f},
		{0f, 0f, 0f, 0f}};
	public Mesh mesh;

	private float maxHeight = 0;

	public float GetHeight(int id) {
		return heightMap[id / x, id % y];
	}
	public float GetHeight(int x, int y) {
		return heightMap[x, y];
	}

	public Vector3 GetPoint(int id) {
		return new Vector3(id / x, id % y, heightMap[id / x, id % y]);
	}
	public Vector3 GetPoint(int x, int y) {
		return new Vector3(x, y, heightMap [x, y]);
	}


	public int GetLengthX() {
		return x;
	}
	public int GetLengthY() {
		return y;
	}
	public int GetLength() {
		return heightMap.Length;
	}

	public float GetMaxHeight()
	{
		if (maxHeight == 0f)
		{
			float max = heightMap[0, 0];
			float min = heightMap[0, 0];
			for (int i = 0; i < x; i++)
			{
				for (int j = 0; j < y; j++)
				{
					float val = heightMap[i, j];
					if (val > max) {
						max = val;
					}
					if (val < min) {
						min = val;
					}
				}
			}
			maxHeight = max - min;
			Debug.Log(maxHeight);
		}
		return maxHeight;
	}
}
