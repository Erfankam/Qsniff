#! /usr/bin/ruby
class Qclient
	attr_writer :personal_info_dict
	attr_reader :personal_info_dict
    @personal_info_dict  = {
					'name'			=> 'Erfan',
					'family'		=> '',
                    'mob_number'    => '',
                    'address'       => '',
                    'sex'           => '',
                    'rank'          => '',
					'info_dir'		=> '',
                    }

    last_event_dict     = {
                    'last_event'    => '',
                    'last_post'     => '',
                    'last_story'    => '',
                    }

    last_post_info      = {
                    'date_time'     => '',
                    'status'        => '',
                    'message'       => '',
                    }

end

Qclient_info_list =	[{
					'name'		=> 'Erfan',
					'family'	=> 'EsmayiliBarzi',
					'mob_number' => '09127017011',
					'sex'	=> 'mail',
					'rank'	=> 'admin',
					'info_dir' => 'Erfan_EsmayiliBarzi',
					'address'	=> 'Mahallati st',
					}
					]
Qclient_info_list.entries.each do |client|
	newQclient = Qclient.new
	client.keys.each do |key|
		puts key
		newQclient.personal_info_dict[key] = client[key]
	end
end

