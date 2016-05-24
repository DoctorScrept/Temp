using UnityEngine;

//use indexes
public class Surface
{
	public int x = 4;
	public int y = 4;

	private float xyScale = 1f;
	private float heightScale = 1f;

	public Mesh mesh;
	
	private float maxHeight = 0f;

//	public float[,] heightMap = { 	
//		{1f, 0f, 0f, 0f},
//		{0f, 2f, 0f, 0f},
//		{0f, 0f, 0f, 0f},
//		{0f, 0f, 0f, 0f}};

	public float[,] heightMap = { 	
		{0f, 0f, 0f, 0f},
		{0f, 0f, 0f, 0f},
		{0f, 0f, 2f, 0f},
		{0f, 0f, 0f, 1f}};
	
	public Surface(int xLength, int yLength) {
		x = xLength;
		y = yLength;
	}
	public Surface() {
//		x = xLength;
//		y = yLength;
	}

	public void SetData(float[] data)
	{
		heightMap = new float[x, y];
		int n = 0;
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				heightMap[i, j] = data[n++];
			}
		}
	}


	public void SetXYScale(float scale) {
		xyScale = scale;
	}

	public float GetXYScale() {
		return xyScale;
	}

	public void SetHeighScale(float scale) {
		heightScale = scale;
	}
	
	public float GetHeighScale() {
		return heightScale;
	}

	public float GetHeight(int id) {
		return heightMap[id / x, id % y];
	}

	public float GetHeight(int x, int y) {
		return heightMap[x, y];
	}
	
	public Vector3 GetPoint(int id) {
		return new Vector3(id / x * xyScale, heightMap[id / x, id % y] * heightScale, (id % y) * xyScale);
	}

	public Vector3 GetPoint(int x, int y) {
		return new Vector3(x * xyScale, heightMap [x, y] * heightScale, y * xyScale);
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
		}
		return maxHeight;
	}
}
