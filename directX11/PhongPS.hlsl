cbuffer LightCBuf
{
	float3 lightPos;
};

static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	// ライト頂点データ
	const float3 vToL = lightPos - worldPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;
	// 拡散減衰量
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// 拡散強度
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	// 最終的な色
	return float4(saturate((diffuse + ambient) * materialColor), 1.0f);
}