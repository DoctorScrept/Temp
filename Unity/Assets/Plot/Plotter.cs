using UnityEngine;

public class Plotter : MonoBehaviour
{
	public Transform centerTransform;
	public MeshFilter[] slots;
	private Surface[] surfaces = new Surface[1];
	
	public GameObject gridConnectionPrefab;

	public Transform tempGrid;


	private Vector3 V(float x, float y, float z)
	{
		return new Vector3(x, y, z);
	}
	private void Test()
	{
		CreateConnetion(V(0f, 0f, 0f), V(1f, 0f, 0f));
		CreateConnetion(V(0f, 0f, 0f), V(0f, 1f, 0f));
		CreateConnetion(V(0f, 0f, 0f), V(0f, 0f, 1f));
		CreateConnetion(V(1f, 0f, 0f), V(1f, 1f, 0f));
		CreateConnetion(V(1f, 0f, 0f), V(1f, 0f, 1f));
		CreateConnetion(V(1f, 1f, 0f), V(1f, 1f, 1f));
		CreateConnetion(V(1f, 0f, 1f), V(1f, 1f, 1f));
		CreateConnetion(V(0f, 1f, 0f), V(1f, 1f, 0f));
		CreateConnetion(V(0f, 1f, 0f), V(0f, 1f, 1f));
		CreateConnetion(V(0f, 0f, 1f), V(1f, 0f, 1f));
		CreateConnetion(V(0f, 0f, 1f), V(0f, 1f, 1f));
		CreateConnetion(V(0f, 1f, 1f), V(1f, 1f, 1f));

		CreateConnetion(V(0f, 0f, 0f), V(1f, 1f, 1f));
	}


	public void BuildGridBySurface(Surface surface, Transform slot)
	{
		int sizeX = surface.GetLengthX();
		int sizeY = surface.GetLengthY();

		for (int x = 0; x < sizeX - 1; x++)
		{
			for (int y = 0; y < sizeY - 1; y++)
			{
				CreateConnetion(surface.GetPoint(x, y), surface.GetPoint(x + 1, y)).transform.SetParent(tempGrid);
				CreateConnetion(surface.GetPoint(x, y), surface.GetPoint(x, y + 1)).transform.SetParent(tempGrid);
			}
		}

	}

	private GameObject CreateConnetion(Vector3 from, Vector3 to) {
		GameObject connection = GameObject.Instantiate(gridConnectionPrefab);
		SetConnetction(connection, from, to);
		return connection;
	}

	private void SetConnetction(GameObject connection, Vector3 from, Vector3 to)
	{
		Vector3 center = (to + from) / 2;
		connection.transform.position = center;
		
		connection.transform.LookAt(to);
		Vector3 cur = connection.transform.rotation.eulerAngles;
		connection.transform.rotation = Quaternion.Euler(cur.x + 90, cur.y, cur.z);

		Vector3 scale = connection.transform.localScale;
		connection.transform.localScale = new Vector3 (scale.x, 0.5f * Vector3.Distance(to, from), scale.z);
	}

	public void DrawToSlot(int slotId, Surface surface)
	{
		slots[slotId].mesh = GenerateMesh(surface);
		//slots[slotId].mesh = GenerateMeshOld(surface);

		surface.slot = slots[slotId].gameObject;
		surfaces[slotId] = surface;

		MeshCollider mc = slots[slotId].gameObject.GetComponent<MeshCollider>();
		mc.sharedMesh = slots[slotId].mesh;

		Vector3 center = mc.bounds.center;
		centerTransform.position = new Vector3(center.x, 0f , center.z);

		BuildGridBySurface(surface, slots[slotId].gameObject.transform);
		Test();
	}

	private Color GetColorByHeight(float height) {
		return Color.Lerp(Color.green, Color.red, height);
	}
	
	public Mesh GenerateMesh(Surface surface)
	{
		int numVertexes = surface.GetLength();
		int numTrianglesPoints = (surface.GetLengthX() - 1) * (surface.GetLengthY() - 1) * 12;
		int sizeX = surface.GetLengthX();
		int sizeY = surface.GetLengthY();

		Vector3[] vertices = new Vector3[numVertexes];
		Color[] colors = new Color[numVertexes];

		for (int i = 0; i < numVertexes; i++)
		{
			vertices[i] = surface.GetPoint(i);
			colors[i] = GetColorByHeight(surface[i] / surface.GetMaxHeight());
		}

		int j = 0;
		int[] triangles = new int[numTrianglesPoints];
		for (int x = 0; x < sizeX - 1; x++)
		{
			for (int y = 0; y < sizeY - 1; y++)
			{
				int firtsQuadId = y * sizeX + x;

				// view from above surface
				triangles[j++] = firtsQuadId + sizeX + 1;
				triangles[j++] = firtsQuadId + sizeX;
				triangles[j++] = firtsQuadId;
				
				triangles[j++] = firtsQuadId + 1;
				triangles[j++] = firtsQuadId + sizeX + 1;
				triangles[j++] = firtsQuadId;

				// bottom view surface
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

	public Surface GetSurfaceBySlot(GameObject slot)
	{
		foreach (Surface s in surfaces) {
			if (s.slot == slot) {
				return s;
			}
		}
		return null;
	}
	
#region OLD_VERSION_AS_EXAMPLE
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

	private Mesh GenerateMeshOld(Surface surface)
	{
		Mesh m = Generate(surface.AsTwoDimensional(), surface.GetLengthX(), surface.GetLengthY(), surface.GetMaxHeight());
		surface.mesh = m;
		return m;
	}
#endregion
}