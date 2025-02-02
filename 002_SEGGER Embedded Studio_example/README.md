## Создаем первый проект в Embedded Studio для GD32F303CCT6  
1. Скачиваем и устанавливаем[ IDE Embedded Studio for ARM V7.32a.](https://www.segger.com/downloads/embedded-studio/#ESforARM "Embedded Studio for ARM")   
2. Скачиваем и устанавливаем необходимые пакеты через менеджер пакетов.  
Tools -> Package Manager -> GigaDevice -> Gd32F30x CPU Support Package -> installed. Версия пакета 2.06.  
3. Создаём проект. File -> New Project. В разделе _Location_ необходимо указать путь до папки проекта. В разделе _Name_ указать имя проекта. Выбрать _A C/C+ executable for GigaDevice GD32F30x_. Жмём _Next_. В вкладке _Target Processor_ выбираем целевое устройство GD32F303CC. Далле жмём _Next_ и _Finish_.
4. Далее необходимо скачать [_GD32F30x Firmware Library._](https://www.gd32mcu.com/en/download/7?kw=GD32F3 "GD32F30x Firmware Library") Актуальная версия 2.2.0 на 2023-12-30.  
5. Перейти в папку проекта. В папку проекта добавить файлы: 
* gd32f30x_gpio.c  
* gd32f30x_it.c  
* gd32f30x_rcu.c  
* systick.c  
Далее перейти в _GD32F30x\Device\Include_. Добавить:
* gd32f30x_it.h
* systick.h  
В IDE добавить выше перечисленные файлы в проект _Sorce Files_ -> _Add Existing File_. Собрать проект нажатием "горячей клавиши F7" -> _Build complete._  
## Установка драйверов  
Скачиваем и устанавливаем [ драйвера.](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack "J-Link Software and Documentation Pack") (J-Link Software and Documentation Pack). Для клонов  JLink V8 USB купленных на aliexpress. Актуальная версия (рабочая версия драйверов) V7.90 от 2023-08-02. С последней версией V7.98c от 2024-08-07 программатор отладчик не определяется!!!  
## Установка IDE.  
Embedded Studio for ARM (legacy). V7.32a [2023-11-29]...  
## Установка тёмной темы Embedded Studio IDE.  
To enable dark mode go to Tools->Options->Environment->User Interface->Theme and set it to Dark...  