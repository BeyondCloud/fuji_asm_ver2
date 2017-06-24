void ADC_Task(void const * argument)
{
 // Tpad_Init();
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_val,1);
  HAL_ADC_Start(&hadc1);

  uint16_t buf_i=0,i;
  //Assume 42MHz sample rate---> down scale to 420Hz
	char str[32],buf_str[32];
	int use_buf=0;

	for(;;)
  {

		  sprintf(str,"%d",ADC_val);//some thing must written here or print wont work...

		if(smp_cnt>=10)
		{
			if(buf_i==320)
			{
				use_buf = !use_buf;
				for(i=0;i<(320-1);i++)
				{
					LCD_DrawLine( i,ADC_buffer[use_buf][i],i,ADC_buffer[use_buf][i+1],Black);
				}
				use_buf = !use_buf;
				
				for(i=0;i<(320-1);i++)
				{
					LCD_DrawLine( i,ADC_buffer[use_buf][i],i,ADC_buffer[use_buf][i+1],Green);
				}
				buf_i=0;
				use_buf = !use_buf;
				
				
			}

				ADC_buffer[use_buf][buf_i] = (ADC_val)*240/4096;

			buf_i++;
			smp_cnt = 0;
		}

   }




}