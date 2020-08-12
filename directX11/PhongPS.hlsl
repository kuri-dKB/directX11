cbuffer LightCBuf
{
	float3 lightPos;
};

static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.15f, 0.15f, 0.15f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 1.0f;
static const float attQuad = 1.0f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	// ���C�g���_�f�[�^
	const float3 vToL = lightPos - worldPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;
	// �g�U������
	const float att = attConst + attLin * distToL + attQuad * (distToL * distToL);
	// �g�U���x
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	// �ŏI�I�ȐF
	return float4(saturate(diffuse + ambient), 1.0f);
}