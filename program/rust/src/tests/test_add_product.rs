use std::mem::size_of;

use crate::c_oracle_header::{
    MappingAccount,
    ProductAccount,
    PythAccount,
    PC_MAGIC,
    PC_MAP_TABLE_SIZE,
    PC_PROD_ACC_SIZE,
    PC_VERSION,
};
use crate::deserialize::{
    initialize_pyth_account_checked,
    load_account_as,
    load_checked,
};
use crate::error::OracleError;
use crate::instruction::{
    CommandHeader,
    OracleCommand,
};
use crate::processor::process_instruction;
use crate::tests::test_utils::AccountSetup;
use crate::utils::{
    clear_account,
    pubkey_equal,
};
use bytemuck::bytes_of;
use num_traits::ToPrimitive;
use solana_program::account_info::AccountInfo;
use solana_program::clock::Epoch;
use solana_program::program_error::ProgramError;
use solana_program::pubkey::Pubkey;
use solana_program::rent::Rent;


#[test]
fn test_add_product() {
    let hdr: CommandHeader = OracleCommand::AddProduct.into();
    let instruction_data = bytes_of::<CommandHeader>(&hdr);

    let program_id = Pubkey::new_unique();

    let mut funding_setup = AccountSetup::new_funding();
    let funding_account = funding_setup.to_account_info();

    let mut mapping_setup = AccountSetup::new::<MappingAccount>(&program_id);
    let mapping_account = mapping_setup.to_account_info();
    initialize_pyth_account_checked::<MappingAccount>(&mapping_account, PC_VERSION).unwrap();

    let mut product_setup = AccountSetup::new::<ProductAccount>(&program_id);
    let product_account = product_setup.to_account_info();

    let mut product_setup_2 = AccountSetup::new::<ProductAccount>(&program_id);
    let product_account_2 = product_setup_2.to_account_info();

    assert!(process_instruction(
        &program_id,
        &[
            funding_account.clone(),
            mapping_account.clone(),
            product_account.clone()
        ],
        instruction_data
    )
    .is_ok());

    {
        let product_data = load_account_as::<ProductAccount>(&product_account).unwrap();
        let mapping_data = load_checked::<MappingAccount>(&mapping_account, PC_VERSION).unwrap();

        assert_eq!(product_data.header.magic_number, PC_MAGIC);
        assert_eq!(product_data.header.version, PC_VERSION);
        assert_eq!(
            product_data.header.account_type,
            ProductAccount::ACCOUNT_TYPE.to_u32().unwrap()
        );
        assert_eq!(product_data.header.size, size_of::<ProductAccount>() as u32);
        assert_eq!(mapping_data.number_of_products, 1);
        assert_eq!(
            mapping_data.header.size,
            (MappingAccount::INITIAL_SIZE + 32)
        );
        assert!(pubkey_equal(
            &mapping_data.product_list[0],
            &product_account.key.to_bytes()
        ));
    }

    assert!(process_instruction(
        &program_id,
        &[
            funding_account.clone(),
            mapping_account.clone(),
            product_account_2.clone()
        ],
        instruction_data
    )
    .is_ok());
    {
        let mapping_data = load_checked::<MappingAccount>(&mapping_account, PC_VERSION).unwrap();
        assert_eq!(mapping_data.number_of_products, 2);
        assert_eq!(
            mapping_data.header.size,
            (MappingAccount::INITIAL_SIZE + 2 * 32)
        );
        assert!(pubkey_equal(
            &mapping_data.product_list[1],
            &product_account_2.key.to_bytes()
        ));
    }

    // invalid account size
    let product_key_3 = Pubkey::new_unique();
    let mut product_balance_3 = Rent::minimum_balance(&Rent::default(), PC_PROD_ACC_SIZE as usize);
    let mut prod_raw_data_3 = [0u8; PC_PROD_ACC_SIZE as usize - 1];
    let product_account_3 = AccountInfo::new(
        &product_key_3,
        true,
        true,
        &mut product_balance_3,
        &mut prod_raw_data_3,
        &program_id,
        false,
        Epoch::default(),
    );
    assert_eq!(
        process_instruction(
            &program_id,
            &[
                funding_account.clone(),
                mapping_account.clone(),
                product_account_3.clone()
            ],
            instruction_data
        ),
        Err(OracleError::InvalidSignableAccount.into())
    );

    // test fill up of mapping table
    clear_account(&mapping_account).unwrap();
    initialize_pyth_account_checked::<MappingAccount>(&mapping_account, PC_VERSION).unwrap();

    for i in 0..PC_MAP_TABLE_SIZE {
        clear_account(&product_account).unwrap();

        assert!(process_instruction(
            &program_id,
            &[
                funding_account.clone(),
                mapping_account.clone(),
                product_account.clone()
            ],
            instruction_data
        )
        .is_ok());
        let mapping_data = load_checked::<MappingAccount>(&mapping_account, PC_VERSION).unwrap();
        assert_eq!(
            mapping_data.header.size,
            MappingAccount::INITIAL_SIZE + (i + 1) * 32
        );
        assert_eq!(mapping_data.number_of_products, i + 1);
    }

    clear_account(&product_account).unwrap();

    assert_eq!(
        process_instruction(
            &program_id,
            &[
                funding_account.clone(),
                mapping_account.clone(),
                product_account.clone()
            ],
            instruction_data
        ),
        Err(ProgramError::InvalidArgument)
    );

    let mapping_data = load_checked::<MappingAccount>(&mapping_account, PC_VERSION).unwrap();
    assert_eq!(mapping_data.number_of_products, PC_MAP_TABLE_SIZE);
}
