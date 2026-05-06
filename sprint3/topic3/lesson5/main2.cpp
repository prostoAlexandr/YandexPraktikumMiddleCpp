void PanelBar::RepositionExpandedPanels(Panel *fixed_panel, int fixed_index) {

    // // Находим панель правее зафиксированной панели
    // const int center_x = fixed_panel->cur_panel_center();
    // auto panel_it = std::find_if(expanded_panels_.begin(), expanded_panels_.end(),
    //                              [center_x](const auto &p) { return center_x < p->cur_panel_center(); });

    // //
    // // Если панель найдена - перегруппировываем все панели
    // //
    // // До:    [           fixed_index,     new_panel_index         ]
    // // После: [           new_panel_index, fixed_index             ]
    // //
    // if (panel_it != expanded_panels_.end()) {
    //     int new_panel_index = std::distance(expanded_panels_.begin(), panel_it);
    //     ref_ptr<Panel> ref = expanded_panels_[fixed_index];

    //     expanded_panels_.erase(expanded_panels_.begin() + fixed_index);
    //     expanded_panels_.insert(expanded_panels_.begin() + new_panel_index, ref);
    // }


    // Находим панель правее зафиксированной панели
    const int center_x = fixed_panel->cur_panel_center();
    auto panel_it = std::find_if(expanded_panels_.begin(), expanded_panels_.end(),
                                 [center_x](const auto &p) { return center_x < p->cur_panel_center(); });

    //
    // Если панель найдена, перегруппировываем все панели
    //
    // До:    [           fixed_index,     new_panel_index         ]
    // После: [           new_panel_index, fixed_index             ]
    //
    std::rotate(expanded_panels_.begin() + fixed_index, expanded_panels_.begin() + fixed_index + 1, panel_it);
}