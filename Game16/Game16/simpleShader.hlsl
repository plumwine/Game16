//��b�̋@�\������shader


//�O���[�o���ϐ�
cbuffer global
{
	matrix g_WVP;  //���[���h����ˉe�܂ł̕ϊ��s��
};

//�o�[�e�b�N�X�V�F�[�_�[�i���_���j
//�Z�}���e�B�N�XPOSITION
float4 VS(float4 Pos:POSITION):SV_POSITION
{
	//�ʒu��mul�֐����g�p����World���W�𒲐�
	Pos = mul(Pos,g_WVP);
    return Pos;
}
//�s�N�Z���V�F�[�_�[�i�s�N�Z���P�ʂŎw��͈͓����ǂ̐F�ŕ`�悷�邩�j
float4 PS(float4 Pos:SV_POSITION) :SV_Target
{
	//����App������s�N�Z���������炤���Ƃ��o���Ȃ��̂ŁA��x�o�[�e�b�N�X�V�F�[�_�[���o�R����
	//����������炤
	//��芸�������ŕ`��
	return float4(1,1,1,1);
}