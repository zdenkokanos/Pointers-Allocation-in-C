for (int i = 0; i < (ciach_count); i++) // načíta do 2D polí súbor ciachovanie
        {
            fgets(line, sizeof(line), file_ciach);
            strncpy((ID_ciach)[i], line, len_id);
            fgets(line, sizeof(line), file_ciach);
            strncpy((DATE_ciach)[i], line, len_date);
            fgets(line, sizeof(line), file_ciach);
        }
        // int print_count = 0; // definujem premennú print count ktorú využívam ako počítadlo v nasledujúcom cykle
        //  int result = 0;
        int ciach_all_dates_match = 0;
        for (int i = 0; i < (ciach_count); i++)
        {
            for (int q = 0; q < *p_count; q++)
            {
                