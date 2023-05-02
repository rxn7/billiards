-- DEBUG
vim.api.nvim_set_keymap('n', '<f4>', ':te cmake --build build/debug<cr>', { noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<f28>', ':te cmake --build build/debug --clean-first<cr>',
	{ noremap = true, silent = true })
vim.api.nvim_set_keymap('n', '<f5>', ':te pushd build/debug; ./billiards ; popd<cr>',
	{ noremap = true, silent = true })

vim.cmd [[autocmd BufWritePre * lua vim.lsp.buf.format()]]
